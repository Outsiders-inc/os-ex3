/*
 * IdHandler.h
 *
 *  Created on: Mar 22, 2015
 *      Author: Author: David Gvirts, Shahar Osovsky
 */

#ifndef IDHANDLER_H_
#define IDHANDLER_H_
#include <vector>

using namespace std;

class IdHandler
{
public:
	IdHandler(unsigned int maxNum);
	~IdHandler();
	int getNewId();
	void removeId(int id);
	bool idExists(int id);
private:
	unsigned int _maxNum;
	vector<int> * _idVec;
};


#endif /* IDHANDLER_H_ */
