#pragma once

ref class ScopeLock
{
private:

	Object^ _object;

public:

	ScopeLock(Object ^object) :
		_object(object)
	{
		Monitor::Enter(_object);
	}

	~ScopeLock()
	{
		Monitor::Exit(_object);
	}

};
