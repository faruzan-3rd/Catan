#ifndef __CTN__BOARD_HPP
#define __CTN__BOARD_HPP

#include <map>
#include <vector>
#include <complex>
#include <random>
#include "wastils/config.hpp"
#include "wastils/operator.hpp"
#include "wastils/UI.hpp"
#include "wastils/geometry.hpp"
#include "catan/assets.hpp"
#include "catan/constants.hpp"
#include "SFML/Graphics.hpp"
#include "yaml-cpp/yaml.h"

using vec2f = sf::Vector2f;
using str = std::string;

namespace ctn{
    class Harbor{
        std::string required_mat;
        int required_num;
        double rotation;
        vec2f 
            position,
            mat_position;
        
    
        public:
        Harbor():
            required_mat{ctn::NONE},
            required_num{-1},
            rotation{0},
            position{vec2f(0, 0)},
            mat_position{vec2f(0, 0)}
        {}
        Harbor(
            const std::string& req_mat_, 
            const int& req_num_, 
            const vec2f& mat_offset_, 
            const vec2f& pl1, 
            const vec2f& pl2,
            const vec2f& position_);

        vec2f get_position() const {return position; }
        vec2f get_mat_position() const {return mat_position; }
        const str& get_required_mat() const {return required_mat; }
        int get_required_num() const {return required_num; }
        double get_rotation() const {return rotation; }
    };
    
    class Place{
        vec2f pos;
        std::vector<int> connected_to;
        std::vector<int> adjacent_tiles;
        int harbor_id;
        int id;
        str
            house_type,
            color;

        public:
        Place():
            pos{vec2f(0, 0)},
            connected_to{{}},
            adjacent_tiles{},
            harbor_id{-1},
            id{-1},
            house_type{ctn::NONE},
            color{ctn::GRAY}
        {}
        Place(
                const vec2f& pos_, 
                const std::vector<int>& connected_to_, 
                const int& id_,
                const str& color_):
            pos{pos_},
            connected_to{connected_to_},
            adjacent_tiles{},
            harbor_id{-1},
            id{id_},
            house_type{ctn::NONE},
            color{color_}
        {
        }

        bool is_clicked(const vec2f& mouse_pos) const;
        vec2f get_position() const {return pos; }
        int get_harbor() const {return harbor_id; }
        void set_harbor(int harbor_id_) {harbor_id = harbor_id_; }
        int get_id() const {return id; }
        void add_adj_tile(int tile_id) {adjacent_tiles.push_back(tile_id); }
        const str& get_color() const {return color; }
        const str& get_type() const {return house_type; }
        void set_settlement(const str& type_, const str& color_){house_type = type_; color = color_; }
        std::vector<int> get_adjacent_tiles() const{return adjacent_tiles; }
    };

    class Path{
        str 
            path_type,
            color;
        vec2f pos;
        int place1, place2;


        public:
        Path():
            path_type{ctn::NONE},
            color{ctn::GRAY},
            pos{0, 0},
            place1{-1},
            place2{-1}
        {}
        Path(
            const str& path_type_, 
            const vec2f& pos_,
            const str& color_,
            int pl1,
            int pl2):
                path_type{path_type_},
                color{color_},
                pos{pos_},
                place1{pl1},
                place2{pl2}
        {}

        bool is_clicked(const vec2f& mouse_pos) const;
        vec2f get_position() const {return pos; }
        const str& get_path_type() const {return path_type; }
        const str& get_color() const {return color; }
        int get_place1() const {return place1; }
        int get_place2() const {return place2; }
        void set_path(const str& color_){color = color_;}
    };


    struct PathData{
        int to;
        int path_id;

        PathData(){}
        PathData(int to_, int path_id_){
            to = to_; path_id = path_id_;
        }
    };

    class BoardTile{
        std::string tile_type;
        vec2f pos;
        int token;

        public:
        BoardTile():
            tile_type{ctn::NONE},
            pos{vec2f(0, 0)},
            token{0}
        {}
        BoardTile(
                const std::string& tile_type_, 
                const vec2f& pos_):
            tile_type{tile_type_},
            pos{pos_},
            token{0}
        {
        }

        vec2f get_position() const {return pos; }
        int get_token() const {return token; }
        void set_token(const int& token_) {token = token_; }
        const str& get_tile_type() const {return tile_type; }

    };
    
    class Board{
        YAML::Node config;

        std::vector<ctn::Place> places;
        std::vector<ctn::Harbor> harbors;
        std::vector<ctn::Path> paths;
        std::vector<ctn::BoardTile> tiles;
        std::vector<std::vector<ctn::PathData>> graph;
        int robber_tile;

        public:
        Board() = default;

        void generate_board(const YAML::Node& config_);
        void generate_graph();
        void generate_harbors();
        void generate_tiles(const YAML::Node& config_);
        void attribute_resources();
        int get_clicked_place(const vec2f& mouse_pos) const;
        int get_clicked_path(const vec2f& mouse_pos) const;
        int get_robber_tile() const{return robber_tile; }

        const std::vector<ctn::Place>& get_places() const{return places; }
        const std::vector<ctn::Harbor>& get_harbors() const{return harbors; }
        const std::vector<ctn::Path>& get_paths() const{return paths; }
        const std::vector<ctn::BoardTile>& get_tiles() const{return tiles; }
        const std::vector<std::vector<ctn::PathData>>& get_graph() const{return graph; }

        void build_settlement(int id, const str& type_, const str& color);
        void build_path(int id, const str& color);
        std::vector<str> get_obtainable_resources(int settlement_id);

        bool can_build_settlement_here(const str& color, int id, bool is_setup_phase) const;
        bool can_build_path_here(const str& color, int id, int required_adj_settlement=-1) const;

        private:
        void make_path_if_exist(Place& pl1, Place& pl2, const std::vector<vec2f>& directions);
        vec2f is_connected(Place& pl1, Place& pl2, const std::vector<vec2f>& directions, int max_radius=5);
        const str get_path_type(const vec2f& dir) const;
    };
}


#endif
