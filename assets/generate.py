#! /usr/bin/env python3

import sys
def encode_to_string_data(image_data):
    """
        0000: A
        0001: B
        0010: C
        0011: D
        0100: E
        0101: F
        0110: G
        0111: H
        1000: I
        1001: J
        1010: K
        1011: L
        1100: M
        1101: N
        1110: O
        1111: P
    """
    map_from_four_bits = [
            'A', 'B', 'C', 'D',
            'E', 'F', 'G', 'H',
            'I', 'J', 'K', 'L',
            'M', 'N', 'O', 'P']
    result_string = ""
    for i in image_data:
        result_string += map_from_four_bits[(i & 0b11110000) >> 4]
        result_string += map_from_four_bits[i & 0b00001111]
    return result_string

class Asset:
    def __init__(self, name, position, size):
        self.name = name
        self.position = position
        self.size = size
class Coord:
    def __init__(self, x, y):
        self.x = x
        self.y = y
class Size:
    def __init__(self, w, h):
        self.w = w
        self.h = h

assets_list = [ # (asset_name, (position, size))
    Asset("base", Coord(0, 0), Size(672, 224)),
    Asset("background_day", Coord(672, 0), Size(576, 1024)),
    Asset("background_night", Coord(1248, 0), Size(576, 1024)),
    Asset("digit_0", Coord(0, 224), Size(48, 72)),
    Asset("digit_1", Coord(48, 224), Size(32, 72)),
    Asset("digit_2", Coord(80, 224), Size(48, 72)),
    Asset("digit_3", Coord(128, 224), Size(48, 72)),
    Asset("digit_4", Coord(176, 224), Size(48, 72)),
    Asset("digit_5", Coord(224, 224), Size(48, 72)),
    Asset("digit_6", Coord(272, 224), Size(48, 72)),
    Asset("digit_7", Coord(320, 224), Size(48, 72)),
    Asset("digit_8", Coord(368, 224), Size(48, 72)),
    Asset("digit_9", Coord(416, 224), Size(48, 72)),
    Asset("bluebird_downflap", Coord(0, 296), Size(68, 48)),
    Asset("bluebird_midflap", Coord(68, 296), Size(68, 48)),
    Asset("bluebird_upflap", Coord(136, 296), Size(68, 48)),
    Asset("redbird_downflap", Coord(204, 296), Size(68, 48)),
    Asset("redbird_midflap", Coord(272, 296), Size(68, 48)),
    Asset("redbird_upflap", Coord(340, 296), Size(68, 48)),
    Asset("yellowbird_downflap", Coord(408, 296), Size(68, 48)),
    Asset("yellowbird_midflap", Coord(476, 296), Size(68, 48)),
    Asset("yellowbird_upflap", Coord(544, 296), Size(68, 48)),
    Asset("pipe_green", Coord(0, 344), Size(104, 640)),
    Asset("pipe_red", Coord(104, 344), Size(104, 640)),
    Asset("getready", Coord(208, 344), Size(368, 534)),
    Asset("gameover", Coord(208, 878), Size(384, 84))
]

with open("assets/assets.png", "rb") as f:
    encoded_string_data = encode_to_string_data(f.read())
    output = open("include/encoded_assets_png.txt", "w")
    output.write('"')
    output.write(encoded_string_data)
    output.write('"')
    output.close()

with open("include/assets_h_generated_assets_list.txt", "w") as output:
    fmt_string = '''
private: static SDL_Surface *{0}_surface_; SDL_Texture *{0}_texture_;
public: static SDL_Surface *{0}_surface() {{ assert(surface_initialized); return {0}_surface_; }} SDL_Texture *{0}_texture() const {{ assert(initialized); return {0}_texture_; }}
'''
    for asset in assets_list:
        output.write(fmt_string.format(asset.name));

with open("include/assets_cpp_generated_initializing_assets_list.txt", "w") as output:
    fmt_string = 'load_asset({}, {}, {}, {}, {})\n'
    for asset in assets_list:
        output.write(fmt_string.format(asset.name, asset.position.x, asset.position.y, asset.size.w, asset.size.h));

with open("include/assets_cpp_generated_free_assets_list.txt", "w") as output:
    fmt_string = 'free_asset({})\n'
    for asset in assets_list:
        output.write(fmt_string.format(asset.name));
