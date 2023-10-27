#pragma once
class PostPixelate : public PostProcessingMaterial
{
public:
	PostPixelate();
	~PostPixelate() override = default;
	PostPixelate(const PostPixelate& other) = delete;
	PostPixelate(PostPixelate&& other) noexcept = delete;
	PostPixelate& operator=(const PostPixelate& other) = delete;
	PostPixelate& operator=(PostPixelate&& other) noexcept = delete;

protected:
	void Initialize(const GameContext& /*gameContext*/) override {}

};

