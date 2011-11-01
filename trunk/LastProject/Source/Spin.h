#ifndef _SPIN_H_
#define _SPIN_H_


class Spin
{
private:
	Spin(){}

	Spin( const Spin& rhs );
	Spin& operator=( const Spin& rhs );


public:
	virtual ~Spin(){}

	static Spin& GetInstance();

	virtual VOID Enter( Miner* pMiner );
	virtual VOID Execute( Miner* pMiner );
	virtual VOID Exit( Miner* pMiner );
};


#endif