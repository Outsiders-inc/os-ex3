int attach_now(int block_num)
{
	try
	{
		if (!gIdHandler->idExists(block_num))
		{
			return -2;
		}
		Node * toAttach = find_pre_hashed_block(block_num);
		if (toAttach != NULL)
		{
			pthread_mutex_lock(&mutex);
			toAttach->setCryptoData(
					generate_hash(toAttach->getData(), toAttach->getLength(),
							generate_nonce(toAttach->getID(),
									toAttach->getFather()->getID())));
			gLeaves.push_back(toAttach);
			gPreHash->remove(toAttach);
			pthread_mutex_unlock(&mutex);
			gNumOfBlk++;
		}
	} catch (exception& e)
	{
		// if "OTHER ERRORS" RETURN -1
		return -1;
	}
	return 0;
}
