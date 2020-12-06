template <typename T>
struct SegmentTree
{
public:
	typedef T (*Operation)(T, T);


	SegmentTree(size_t size, Operation operation, T operationDefaultResult)
	{
		_operationDefaultResult = operationDefaultResult;
		_operation = operation;

		_initialize(size);
	}

	SegmentTree(T* begin, T* end, Operation operation, T operationDefaultResult)
	{
		_operationDefaultResult = operationDefaultResult;
		_operation = operation;

		_initialize(end - begin);
		_build(begin, end - begin);
	}

	void Set(size_t index, T value)
	{
		_set(index, value, 0, 0, _size);
	}

	T ResultOn(size_t start, size_t end)
	{
		return _resultOn(start, end, 0, 0, _size);
	}

protected:
	void _initialize(size_t size)
	{
		_size = 1;
		while (_size < size)
			_size *= 2;

		size_t treeSize = 2 * _size + 1;
		_tree = new T[treeSize];
		for (size_t i = 0; i < treeSize; i++)
			_tree[i] = _operationDefaultResult;
	}

	void _build(T* arr, size_t size)
	{
		_build(arr, size, 0, 0, _size);
	}

	void _build(T* arr, size_t size, size_t node, size_t segmentStart, size_t segmentEnd)
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

	void _set(size_t index, T value, size_t node, size_t segmentStart, size_t segmentEnd)
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
	
	T _resultOn(size_t start, size_t end, size_t node, size_t segmentStart, size_t segmentEnd)
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


	Operation _operation;
	T _operationDefaultResult;
	size_t _size;
	T* _tree;
};
