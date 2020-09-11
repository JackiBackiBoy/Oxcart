#ifndef MATH_HEADER
#define MATH_HEADER

class Math
{
public:
	~Math();

	inline static float ToRadians(const float& someDegrees) { return someDegrees * (Pi / 180); }

	static constexpr float Pi = 3.1415927f;
private:
	Math();
};
#endif