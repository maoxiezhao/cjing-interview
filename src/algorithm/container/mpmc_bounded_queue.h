#pragma once

/*  Multi-producer/multi-consumer bounded queue.
 *  Copyright (c) 2010-2011, Dmitry Vyukov. All rights reserved.
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *     1. Redistributions of source code must retain the above copyright notice, this list of
 *        conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice, this list
 *        of conditions and the following disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *  THIS SOFTWARE IS PROVIDED BY DMITRY VYUKOV "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 *  DMITRY VYUKOV OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *  The views and conclusions contained in the software and documentation are those of the authors and should not be interpreted
 *  as representing official policies, either expressed or implied, of Dmitry Vyukov.
 *  http://www.1024cores.net/home/lock-free-algorithms/queues/bounded-mpmc-queue
 */

namespace Cjing3D
{
	// based on atomic operation
	template<typename T>
	class MPMCBoundedQueue
	{
	public:
		MPMCBoundedQueue() = default;

		MPMCBoundedQueue(I32 size) :
			mBuffer(CJING_NEW_ARR(Cell, size)),
			mBufferMask(size - 1)
		{
			assert((size >= 2) && ((size & (size - 1)) == 0));
			Concurrency::Barrier();
			for (size_t i = 0; i != size; i += 1) {
				mBuffer[i].mSequence = i;
			}
			mEnqueuePos = 0;
			mDequeuePos = 0;
		}

		MPMCBoundedQueue(MPMCBoundedQueue&& other)
		{
			std::swap(mBuffer, other.mBuffer);
			std::swap(mBufferMask, other.mBufferMask);
			std::swap(mEnqueuePos, other.mEnqueuePos);
			std::swap(mDequeuePos, other.mDequeuePos);
		}

		MPMCBoundedQueue& operator=(MPMCBoundedQueue&& other)
		{
			std::swap(mBuffer, other.mBuffer);
			std::swap(mBufferMask, other.mBufferMask);
			std::swap(mEnqueuePos, other.mEnqueuePos);
			std::swap(mDequeuePos, other.mDequeuePos);
			return *this;
		}

		~MPMCBoundedQueue()
		{
			CJING_DELETE_ARR(mBuffer, mBufferMask + 1);
		}

		void Reset(I32 size)
		{
			if (mBuffer != nullptr) {
				CJING_DELETE_ARR(mBuffer, mBufferMask + 1);
			}

			mBuffer = CJING_NEW_ARR(Cell, size);
			mBufferMask = size - 1;

			Concurrency::Barrier();
			for (size_t i = 0; i != size; i += 1) {
				mBuffer[i].mSequence = i;
			}
			mEnqueuePos = 0;
			mDequeuePos = 0;

		}

		bool Enqueue(const T& data)
		{
			Cell* cell = nullptr;
			I32 pos = Concurrency::AtomicCmpExchange(&mEnqueuePos, 0, 0);
			while (true)
			{
				cell = &mBuffer[pos & mBufferMask];
				I32 seq = Concurrency::AtomicCmpExchangeAcquire(&cell->mSequence, 0, 0);
				I32 dif = seq - pos;
				if (dif == 0)
				{
					if (Concurrency::AtomicCmpExchangeAcquire(&mEnqueuePos, pos + 1, pos) == pos) {
						break;
					}
				}
				else if (dif < 0) {
					return false;
				}
				else {
					pos = Concurrency::AtomicCmpExchange(&mEnqueuePos, 0, 0);
				}
			}
			cell->mData = data;
			Concurrency::Barrier();
			cell->mSequence = pos + 1;
			return true;
		}

		bool Dequeue(T& data)
		{
			Cell* cell = nullptr;
			I32 pos = Concurrency::AtomicCmpExchange(&mDequeuePos, 0, 0);
			while (true)
			{
				cell = &mBuffer[pos & mBufferMask];
				I32 seq = Concurrency::AtomicCmpExchangeAcquire(&cell->mSequence, 0, 0);
				I32 dif = seq - (pos + 1);
				if (dif == 0)
				{
					if (Concurrency::AtomicCmpExchangeAcquire(&mDequeuePos, pos + 1, pos) == pos) {
						break;
					}
				}
				else if (dif < 0) {
					return false;
				}
				else {
					pos = Concurrency::AtomicCmpExchange(&mDequeuePos, 0, 0);
				}
			}
			data = cell->mData;
			Concurrency::Barrier();
			cell->mSequence = pos + mBufferMask + 1;
			return true;
		}

	private:
		MPMCBoundedQueue(const MPMCBoundedQueue& queue) = delete;
		MPMCBoundedQueue& operator=(const MPMCBoundedQueue& queue) = delete;

		struct Cell
		{
			volatile I32 mSequence = 0;
			T mData;
		};

		static size_t const CACHE_LINE_SIZE = 64;
		typedef char CacheLinePad[CACHE_LINE_SIZE];

		CacheLinePad mPad0 = { 0 };
		Cell* mBuffer = nullptr;
		I32   mBufferMask = 0;
		CacheLinePad mPad1 = { 0 };
		volatile I32 mEnqueuePos = 0;
		CacheLinePad mPad2 = { 0 };
		volatile I32 mDequeuePos = 0;
		CacheLinePad mPad3 = { 0 };
	};
}