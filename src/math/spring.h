#pragma once

// https://zhuanlan.zhihu.com/p/43238446?ivk_sa=1024320u
// https://zhuanlan.zhihu.com/p/109928859
// https://zhuanlan.zhihu.com/p/121562244
// https://www.youtube.com/watch?v=lw5U4sLN5Qk

class SpringDamper
{
private:
	vec4	Head;
	vec4	Tail;
	vec4	Velocity;
	float	SpringConstant;
	float	DampConstant;

    const float MAX_TIMESTEPS = 10;
    const float EPSILON = 0.0001f;
public:
    void Update(float remainingTime)
    {
        vec4 stretch = Tail - Head;
        float step = CalculateTimestep(remainingTime, stretch.length(), Velocity);
        do
        {
            float dt = min(step, remainingTime);
            // Compute Fk and Fd
            vec4 fk = stretch * SpringConstant;
            vec4 fd = Velocity * -DampConstant;
            vec4 f = fk + fd;
            
            // Update velocity
            vec4 newVelocity = Velocity + f * dt;
            // Update the tail position
            Tail += (newVelocity + Velocity) * 0.5f * dt;
            Velocity = newVelocity

            remainingTime -= step;
        } while (remainingTime > EPSILON); 
    }

private:
    float CalculateTimestep(float remainingTime, float stretch, float velocity) 
    {
        if (velocity == 0) 
            return remainingTime;

        float timestepMin = remainingTime / MAX_TIMESTEPS;
        return Clamp(abs(stretch / velocity), timestepMin, remainingTime);
    }

};
