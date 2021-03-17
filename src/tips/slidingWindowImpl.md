
双指针滑动窗口的经典写法。右指针不断往右移，移动到不能往右移动为止(具体条件根据题目而定)。当右指针到最右边以后，开始挪动左指针，释放窗口左边界。
left, right := 0, -1

for left < len(s) {
    if right+1 < len(s) && freq[s[right+1]-'a'] == 0 {
        freq[s[right+1]-'a']++
        right++
    } else {
        freq[s[left]-'a']--
        left++
    }
    result = max(result, right-left+1)
}

