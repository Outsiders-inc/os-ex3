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
