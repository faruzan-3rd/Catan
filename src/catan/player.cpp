#include "catan/player.hpp"


ctn::PlayerInfo ctn::Player::get_info() const{
    return {id, color, owned_resources};
}


void ctn::Player::update(const was::MouseManager& mouse, const Board& board, const GameState& state){
    if(!mouse.button_pressed_down(sf::Mouse::Left)) return;

    // クリックの情報をEventManagerに送る
    const vec2f mouse_pos = mouse.get_mouse_pos_relative();
    int 
        clicked_path = board.get_clicked_path(mouse_pos),
        clicked_settlement = board.get_clicked_place(mouse_pos);

    if(clicked_path != -1){
        calls.select(SelectionType::Path, clicked_path, color);
    }
    if(clicked_settlement != -1){
        calls.select(SelectionType::Settlement, clicked_settlement, color);
    }
}


void ctn::Player::UI_button_input(UIButton button){
    if(button == UIButton::Build){
        calls.confirm();
    }
    else if(button == UIButton::DiceRoll){
        calls.roll_dices();
    }
}
