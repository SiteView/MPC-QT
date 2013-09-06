#ifndef __daemon_h__
#define __daemon_h__

class GenieDaemon
{
public:
	static GenieDaemon *create()
	{
		return new GenieDaemon();
	};
	virtual ~GenieDaemon() {}
	virtual bool start(){return true;}
	virtual void stop(){return;};

protected:
	GenieDaemon() {}
};

class TestCon2
{
public:
	TestCon2();
	~TestCon2();

	int run();

private:
	GenieDaemon *m_daemon;
};

#endif // __daemon_h__
