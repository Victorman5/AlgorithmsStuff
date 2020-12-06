template <typename NodeType>
struct SegmentTree
{
public:
	typedef NodeType (*OperationFunction)(NodeType, NodeType);


	SegmentTree(size_t size, OperationFunction operation, NodeType operationDefaultResult)
	{
		_operationDefaultResult = operationDefaultResult;
		_operation = operation;

		_initialize(size);
	}

	SegmentTree(NodeType* begin, NodeType* end, OperationFunction operation, NodeType operationDefaultResult)
	{
		_operationDefaultResult = operationDefaultResult;
		_operation = operation;

		_initialize(end - begin);
		_build(begin, end - begin);
	}

	void Set(size_t index, NodeType value)
	{
		_set(index, value, 0, 0, _originalSize);
	}

	NodeType Get(size_t index) 	
	{
		return _tree[_treeSize - _originalSize + index];
	}

	NodeType ResultOn(size_t start, size_t end)
	{
		return _resultOn(start, end, 0, 0, _originalSize);
	}

	NodeType Head()
	{
		return _tree[0];
	}

	size_t OriginalSize()
	{
		return _originalSize;
	}

	NodeType* RawTree()
	{
		return _tree;
	}

	size_t RawTreeSize()
	{
		return _treeSize;
	}

	OperationFunction Operation()
	{
		return _operation;
	}

	NodeType OperationDefaultResult()
	{
		return _operationDefaultResult;
	}

protected:
	void _initialize(size_t size)
	{
		_originalSize = 1;
		while (_originalSize < size)
			_originalSize *= 2;

		_treeSize = 2 * _originalSize - 1;
		_tree = new NodeType[_treeSize];
		for (size_t i = 0; i < _treeSize; i++)
			_tree[i] = _operationDefaultResult;
	}

	void _build(NodeType* arr, size_t size)
	{
		_build(arr, size, 0, 0, _originalSize);
	}

	void _build(NodeType* arr, size_t size, size_t node, size_t segmentStart, size_t segmentEnd)
	{
		if (segmentEnd - segmentStart == 1)
		{
			if (segmentStart < size)
				_tree[node] = arr[segmentStart];
		}
		else
		{
			size_t mid = (segmentStart + segmentEnd) / 2;
			_build(arr, size, 2 * node + 1, segmentStart, mid);
			_build(arr, size, 2 * node + 2, mid, segmentEnd);
			_tree[node] = _operation(_tree[2 * node + 1], _tree[2 * node + 2]);
		}
	}

	void _set(size_t index, NodeType value, size_t node, size_t segmentStart, size_t segmentEnd)
	{
		if (segmentEnd - segmentStart == 1)
		{
			_tree[node] = value;
			return;
		}

		size_t mid = (segmentStart + segmentEnd) / 2;
		if (index < mid)
			_set(index, value, 2 * node + 1, segmentStart, mid);
		else
			_set(index, value, 2 * node + 2, mid, segmentEnd);

		_tree[node] = _operation(_tree[2 * node + 1], _tree[2 * node + 2]);
	}
	
	NodeType _resultOn(size_t start, size_t end, size_t node, size_t segmentStart, size_t segmentEnd)
	{
		if (segmentEnd <= start || segmentStart >= end)
			return _operationDefaultResult;

		if (segmentStart >= start && segmentEnd <= end)
			return _tree[node];

		size_t mid = (segmentStart + segmentEnd) / 2;
		return _operation(
			_resultOn(start, end, 2 * node + 1, segmentStart, mid),
			_resultOn(start, end, 2 * node + 2, mid, segmentEnd));
	}


	OperationFunction _operation;
	NodeType _operationDefaultResult;

	size_t _originalSize;

	size_t _treeSize;
	NodeType* _tree;
};
