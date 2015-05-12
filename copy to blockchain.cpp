/*
 * blockchain.cpp
 *
 *  Created on: May 2015
 *      Author: David Gil Gvirts, Shahar Osovsky
 */

#include "blockchain.h"
#include "general.h"

/*
 * Hashing data and inserting to block
 */
void hash_data()
{
	Node * toHash = gPreHash->front();
	toHash->setCryptoData(
			generate_hash(toHash->getData(), toHash->getLength(),
					generate_nonce(toHash->getID(),
							toHash->getFather()->getID())));
	gPreHash->pop_front();
}

/**
 * Thread function- Chain management
 */
void * block_manager(void * param)
{
	while (gInit)
	{
		if (!gPreHash->empty())
		{
			hash_data();
		}

		break;
	}
	return NULL;
}
//	 pthread_mutex_lock(&mutex);
/* Do work. */
//	pthread_mutex_unlock(&mutex);
/*
 * DESCRIPTION: This function initiates the Block chain, and creates the genesis Block. The
 * genesis Block does not hold any transaction data or hash.
 * This function should be called prior to any other functions as a necessary precondition for
 * their success (all other functions should return with an error otherwise).
 * RETURN VALUE: On success 0, otherwise -1.
 */
int init_blockchain()
{
	if (pthread_create(&gDaemonThread, NULL, block_manager, NULL))
	{
		cerr << THREAD_FAILURE << endl;
		return -1;
	}
// Initiating things, creating genesis and assigning it to the gLeaves
	init_hash_generator();
	gPreHash = new list<Node*>();
	Node* genesis = new Node(0, NULL, 0, 0);
	gLeaves.push_back(genesis);
// gBlockChain = new Chain(genesis);
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

/**
 * Finds the longest chain in the tree, and returns it's leaf.
 */
Node* find_longest_chain()
{
	unsigned int longest = 0;
	Node * longestLeaf;
	for (unsigned int i = 0; i < gLeaves.size(); i++)
	{
		if ((gLeaves.at(i)->getLevel()) > longest)
		{
			longestLeaf = gLeaves.at(i);
			longest = longestLeaf->getLevel();
		}
	}
	return longestLeaf;
}

/**
 * Returns a pointer to a block, which will be the son's father in the tree.
 */
Node* find_father()
{
	Node* longest = find_longest_chain();
	return longest;
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
int add_block(char *data, size_t length) //----------------------------MAYBE FINISHED - V
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
	for (list<Node*>::iterator it = gPreHash->begin(); it != gPreHash->end();
			it++)
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

	Node* block;
	try
	{
		if (!gIdHandler->idExists(block_num))
			{
				return -2;
			}
		block = find_pre_hashed_block(block_num);

		if (block == NULL)
		{
			return 1;
		}
			Node* father = find_father();
			block->setFather(father);
	}
	catch (exception& e)
	{
		// if "OTHER ERRORS" RETURN -1
		return -1;
	}
	return 0;
}
/*
 * DESCRIPTION: This function blocks all other Block attachments, until block_num is added to
 * the chain. The block_num is the assigned value
 * that was previously returned by add_block.
 * RETURN VALUE: If block_num doesn't exist, return -2;
 * In case of other errors, return -1; In case of success or if it is already attached return 0.
 */

int attach_now(int block_num)
{

}

/*
 * DESCRIPTION: Without blocking, check whether block_num was added to the chain.
 * The block_num is the assigned value that was previously returned by add_block.
 * RETURN VALUE: 1 if true and 0 if false. If the block_num doesn't exist, return -2;
 * In case of other errors, return -1.
 */

int was_added(int block_num)
{

}
/*
 * DESCRIPTION: Return how many Blocks were attached to the chain since init_blockchain.
 * If the chain was closed (by using close_chain) and then initialized (init_blockchain) again
 * this function should return the new chain size.
 * RETURN VALUE: On success, the number of Blocks, otherwise -1.
 */

int chain_size()
{

}
/**
 * @ Returns
 */
bool isInPath(Node * elem, vector<Node *> * path)
{
	unsigned int level = path->size() - elem->getLevel() - 1;
	return (path->at(level)->getID() == elem->getID());
}
/*
 * DESCRIPTION: Search throughout the tree for sub-chains that are not the longest chain,
 * detach them from the tree, free the blocks, and reuse the block_nums.
 * RETURN VALUE: On success 0, otherwise -1.
 */

int prune_chain()
{
	vector<Node *> * path = get_longest_path(find_longest_chain());

	unsigned int i;
	for (i = 0; i < gLeaves.size(); i++)
	{
		if (!isInPath(gLeaves[i], path))
		{
			Node * current = gLeaves[i]->getFather();
			delete gLeaves[i];
			gIdHandler->removeId(gLeaves[i]->getID());
			while (!isInPath(current, path))
			{
				Node * temp;
				temp = current;
				current = current->getFather();
				gIdHandler->removeId(temp->getID());
				delete temp;
			}
		}
	}
	return 0;
}

/*
 * DESCRIPTION: Close the recent blockchain and reset the system, so that it is possible to
 * call init_blockchain again. Non-blocking.
 * All pending Blocks should be hashed and printed to terminal (stdout).
 * Calls to library methods which try to alter the state of the Blockchain are prohibited while
 * closing the Blockchain.
 * e.g.: Calling chain_size() is ok, a call to prune_chain() should fail.
 * In case of a system error, the function should cause the process to exit.
 */

void close_chain()
{
	//------------Delete stuff--------
	close_hash_generator();
}

/*
 * DESCRIPTION: The function blocks and waits for close_chain to finish.
 * RETURN VALUE: If closing was successful, it returns 0.
 * If close_chain was not called it should return -2. In case of other error, it should return -1.
 */

int return_on_close()
{

}
