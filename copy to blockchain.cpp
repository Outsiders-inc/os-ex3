void hash_data(bool print)
{
	Node * toHash = gPreHash->front();
	char* crypt = generate_hash(toHash->getData(), toHash->getLength(),
			generate_nonce(toHash->getID(),
					toHash->getFather()->getID()));
	toHash->setCryptoData(crypt);
	if (print)
	{
		cout << crypt << endl;
	}
	else
	{
		gLeaves.push_back(toHash);
		gNumOfBlk++;
	}
	gPreHash->pop_front();
}




void close_chain()
{
	prune_chain();
	//Add flag that this function was called so prune and other func can't be called
	_close = true;
	Node* toDelete;
	Node* father;
	if (gLeaves.size() == 1)
	{
		toDelete = gLeaves.at(0);
		while (toDelete->getID() != 0)
		{
			father = toDelete->getFather();
			gIdHandler->removeId(toDelete->getID());
			gNumOfBlk--;
			delete toDelete;
			toDelete = father;
		}
		delete toDelete;
	}
	else
	{
		exit(-1);
	}
	while(!gPreHash->empty())
	{
		hash_data(PRINT);
	}
	close_hash_generator();
}
