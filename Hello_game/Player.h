#pragma once

#include "config.h"
#include "Entity.h"


namespace ezg {


    class Hero : public ezg::Entity
    {

    public:
        Hero() noexcept
            : Entity(TipeEntity::MainHero)
            , frame_stat(Stat::Stay)
        {

        }

        void load(sf::Texture& texture, float pos_x, float pos_y, float _width, float _height);


        void jump();


        virtual void colisionMap(Direction _dir, TileMap& _map) override;


        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


    private:

        //для анимации (todo)
        float current_frame = 0;
        Stat frame_stat;

    }; // class Hero

} //namespace ezg