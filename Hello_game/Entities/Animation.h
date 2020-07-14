#pragma once

#include <SFML/Graphics.hpp>
#include <map>

namespace ezg {

	/////////////////////////////////////////////////////////////////////////////////////////
	//struct Frame:
	//	stores the rectangle of the frame in the texture and rendering time.
	struct Frame
	{
		Frame() noexcept
			: m_time(0)
			, m_rect()
		{ }

		Frame (float _time, sf::IntRect _rect) noexcept
			: m_time(_time)
			, m_rect(_rect)
		{ }
		

		float m_time;
		sf::IntRect m_rect;

	};


	/////////////////////////////////////////////////////////////////////////////////////////
	//class Animation:
	//	serves to implement animation.
	//	allows you to store multiple animations.
	class Animation
	{
	public:
		//high probability of mistake. 
		//sf::Texture does not tolerate copying or moving. 
		//as a result a white screen is drawn instead of images.
		Animation             (const Animation& _that) = delete;   //not saported
		Animation& operator=  (const Animation&)       = delete;   //not saported
		Animation             (Animation&& _that)      = delete;   //not saported
		Animation& operator=  (Animation&&)            = delete;   //not saported

	public:

		//default constructor (creates an empty class)
		Animation() noexcept;
		

		//texture loading.
		//there is only one texture for all animations
		bool loadTexture(const std::string& _fname)  { return m_texture.loadFromFile(_fname); }
		//Attention! when using this function, make sure that the texture is not destroyed. 
		//otherwise there will be a white square.
		void setTexture(const sf::Texture& _texture) { m_texture = _texture; }


		//creates a new animation.
		//if an animation already exists under '_id' number, then it is cleared.
		void addAnimation(int _id) { m_animations[_id].clear(); }


		//adds a frame to the animation at number _id_animation.
		//the new frame is placed last.
		void addFrame(int _id_animation, sf::IntRect _rect, float _time);


		//make animation number _id_animation active
		//if there is not a single frame in the animation, then it will not be activated
		bool activate(int _id_animation);


		//will return the number of the active animation (otherwise -1)
		inline const int getActive() const noexcept { return is_active ? m_active : -1; }


		//updates frame according to time
		void update(float _time);


		//returns the sprite with the current frame.
		//if no animation is active, then returns an empty slide
		sf::Sprite getSprite() const;



	private:

		bool is_active;
		int m_active;

		float m_time;
		sf::Texture m_texture;

		std::map< int, std::vector<Frame> > m_animations;
		int m_current_frame;

	}; // class Animation


} // namespace ezg