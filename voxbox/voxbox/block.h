enum class BlockType
{
	BlockType_Default = 0,
	BlockType_Grass,
	BlockType_Dirt
};

class Block
{
public:
	Block(BlockType type) { _type = type; }
	~Block() { }

	inline bool isActive() { return _active; }
	inline void setActive(bool state) { _active = state; }
	inline BlockType getType() { return _type; }

private:
	bool _active;
	BlockType _type;
};