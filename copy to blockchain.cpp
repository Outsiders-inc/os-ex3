/*
 * Hashing data and inserting to block
 */
void hash_data()
{
	Node * toHash = gPreHash->front();
	toHash->setCryptoData(generate_hash(toHash->getData(),toHash->getLength(),
			generate_nonce(toHash->getID(), toHash->getFather()->getID())));
	gPreHash->pop_front();
}




/*
 * DESCRIPTION: This function initiates the Block chain, and creates the genesis Block.  The
 * genesis Block does not hold any transaction data or hash.
 * This function should be called prior to any other functions as a necessary precondition for
 * their success (all other functions should return with an error otherwise).
 * RETURN VALUE: On success 0, otherwise -1.
 */
int init_blockchain()
{

	if(pthread_create(&gDaemonThread, NULL, block_manager, NULL)) {
		cerr<<THREAD_FAILURE<<endl;
		return -1;
	}
	// Initiating things, creating genesis and assigning it to the gLeaves
	init_hash_generator();
	gPreHash = new list<Node*>();
	Node* genesis = new Node(0,NULL,0,0);
	gLeaves.push_back(genesis);
//	gBlockChain = new Chain(genesis);
	gInit = true;
	gNumOfBlk = 0;
	gIdHandler = new IdHandler(INT_MAX);
	return 0;
}




/**
 * Returning a vector of Node* representing the path from the given leaf up to the root.
 * THe leaf is at index 0.
 */
vector<Node*> * get_longest_path(Node* leaf)
{
	vector<Node*>* path = new vector<Node*>;
	Node * cur = leaf;
	while (cur->getID() != 0)
	{
		path->push_back(cur);
		cur = cur->getFather();
	}
	return path;
}




/*
 * DESCRIPTION: Ultimately, the function adds the hash of the data to the Block chain.
 * Since this is a non-blocking package, your implemented method should return as soon as
 * possible, even before the Block was actually attached to the chain.
 * Furthermore, the father Block should be determined before this function returns. The father
 * Block should be the last Block of the current longest chain (arbitrary longest chain if
 * there is more than one).
 * Notice that once this call returns, the original data may be freed by the caller.
 * RETURN VALUE: On success, the function returns the lowest available block_num (> 0),
 * which is assigned from now on to this individual piece of data.
 * On failure, -1 will be returned.
 */
int add_block(char *data , size_t length)	//----------------------------MAYBE FINISHED - V
{
	if (!gInit)
	{
		cerr << NO_INIT_ERROR << endl;
		return -1;
	}
	Node* father = find_father();
	unsigned int id = gIdHandler->getNewId();
	Node* newNode = new Node(id, data, length, father);
	// Putting new un-hashed node in the gPreHash list
	gPreHash->push_back(newNode);
	gNumOfBlk++;

	return 0;
}



/**
 * Returns the pre-hashed block with the given block_num (id),
 * If not found - returns NULL.
 */
Node* find_pre_hashed_block(int block_num)
{
	for (list<Node*>::iterator it = gPreHash->begin(); it != gPreHash->end(); it++)
	{
		if ((*it)->getID() == block_num)
		{
			return *it;
		}
	}
	return NULL;
}




/*
 * DESCRIPTION: Without blocking, enforce the policy that this block_num should be attached to the
 * longest chain at the time of attachment of the Block. For clearance, this is opposed to the
 * original add_block that adds the Block to the longest chain during the time that add_block
 * was called.
 * The block_num is the assigned value that was previously returned by add_block.
 * RETURN VALUE: If block_num doesn't exist, return -2; In case of other errors, return -1;
 * In case of success return 0; In case block_num is
 * already attached return 1.
 */

int to_longest(int block_num)
{
	if (!gIdHandler->idExists(block_num))
	{
		return -2;
	}
	Node* block;
	try
	{

	block = find_pre_hashed_block(block_num);
	}
	catch (exception e)
	{
		// if "OTHER ERRORS" RETURN -1
		return -1;
	}
	if (block == NULL)
	{
		return 1;
	}
	try
	{
		Node* father = find_father();
		block->setFather(father);
	}
	catch (exception e)
	{
		// if "OTHER ERRORS" RETURN -1
		return -1;
	}
	return 0;
}
