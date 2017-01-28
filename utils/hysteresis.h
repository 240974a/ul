#pragma once
template <typename T, bool DEFAULT_STATE>
class hysteresis
{
    const static bool UP = true;
    const static bool DOWN = false;
    // ------------------------------
    const T low_, high_;
    bool current_state_ = DEFAULT_STATE;
public:
    hysteresis(T low, T high) : low_(low), high_(high){}
    bool apply_next_value(T value)
    {
        if(value < low_)
            return (current_state_ = DOWN);
        if(value > high_)
            return (current_state_ = UP);
        else
            return current_state_;
    }
};
