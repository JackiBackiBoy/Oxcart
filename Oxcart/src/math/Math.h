#ifndef MATH_HEADER
#define MATH_HEADER

class Math
{
public:
	~Math();

	inline static float ToRadians(const float& someDegrees) { return someDegrees * (Pi / 180); }
	inline static float Lerp(const float& aValue1, const float& aValue2, const float& aPercentage)
	{
		return (1 - aPercentage) * aValue1, + aPercentage * aValue2;
	}

	static constexpr float Pi = 3.1415927f;
private:
	Math();
};
#endif