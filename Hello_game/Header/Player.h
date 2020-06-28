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

        
        //makes the hero jump ...
        void jump() noexcept;


        //determines the interaction of the hero with other objects
        void colision(gsl::not_null <Entity*> _entity, Direction _dir) override;
        

        //draws a hero texture
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


    private:

        //for animation (todo)
        float current_frame = 0;
        Stat frame_stat;


    }; // class Hero


} //namespace ezg