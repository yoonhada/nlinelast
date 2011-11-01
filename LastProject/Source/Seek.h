#ifndef _SEEK_H_
#define _SEEK_H_


class Seek
{
private:
	Seek(){}

	Seek( const Seek& rhs );
	Seek& operator=( const Seek& rhs );


public:
	virtual ~Seek(){}

	static Seek& GetInstance();

	virtual VOID Enter( Miner* pMiner );
	virtual VOID Execute( Miner* pMiner );
	virtual VOID Exit( Miner* pMiner );
};


#endif