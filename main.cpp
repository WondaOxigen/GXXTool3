#include <iostream>
#include "Binary.hpp"
#include "GE.hpp"
#include "GEMath.hpp"
#include "ge_constants.hpp"
#include "color.hpp"

using namespace std;

bool debug = false;



void debug_log(string s)
{
    if(debug)
    cout << s << endl;
}

uint32_t bgr_to_rgba(uint16_t bgr565);
float rad2deg(float angle);

int main(int argc, char* argv[])
{
    cout << hue::light_aqua << "#----------------------------------------#" << hue::reset << endl << endl;
    cout << hue::light_aqua << "#---GXXTool3 by Owocek and wondaoxigen---#" << hue::reset << endl << endl;
    cout << hue::light_aqua << "#----------------------------------------#" << hue::reset << endl << endl;

    if (argc < 2) 
    {
        // Wonda: checking if we have a file 
        std::cout << "No file specified.\n";
        system("pause");
        return 0;
    }

    //string file = "chr01_01_01_1.gxx";
    string file = argv[1];
    cout << hue::black_on_light_yellow << "Reading file:" << hue::reset << " " << file << endl << endl;

    debug_log("grab file");
    vector<unsigned char> gxx = Binary::file_to_uchar(Binary::get_file(file));

    // Wonda: check if file starts with "XXG.01.0OMG."
    if (!(Binary::get_uint32(gxx, 0x00) == 0x2e475858 && Binary::get_uint32(gxx, 0x04) == 0x302e3130 && Binary::get_uint32(gxx, 0x08) == 0x2e474d4f))
    {
        // Wonda: check if file starts with "OMG.00.1PSP"
        if (Binary::get_uint32(gxx, 0x00) == 0x2e474d4f && Binary::get_uint32(gxx, 0x04) == 0x312e3030 && Binary::get_uint32(gxx, 0x08) == 0x00505350)
        {
            std::cout << "This is GMO file.\nUse Noesis to view it." << endl;
        }
        else
        {
        std::cout << "This is not GXX file.\n";
        }
        system("pause");
        return 0;   
    }
    uint32_t section_c = Binary::get_uint32(gxx, 0x10);
    uint32_t section_b = Binary::get_uint32(gxx, 0x28);
    uint32_t section_a = Binary::get_uint32(gxx, 0x30);

    /*{
        //Ask about console output
        string anwser;
        
        while(anwser != "Y" && anwser != "N")
        {
            cout << hue::light_yellow << "Print model data in console?" << hue::reset << " (Y/N): ";
            cin >> anwser;
        }
        if (anwser == "N")
        {
            std::cout.setstate(std::ios_base::failbit);

            //cout.clear whenever output is required
            //std::cout.clear();
        }

    }*/

    debug_log("get main offsets");
    uint32_t main_offsets = Binary::get_uint32(gxx, section_c);

    debug_log("start loop");

    // Step 1: Texture name
    string texture;
    {
        uint32_t offset_1 = Binary::get_uint32(gxx, section_c + 0x08);
        uint32_t offset_2 = Binary::get_uint32(gxx, offset_1);
        uint32_t offset_3 = Binary::get_uint32(gxx, offset_2 + 0x04);
        texture = Binary::get_string(gxx, offset_3);
        cout << hue::black_on_light_yellow << "Texture:" << hue::reset << " " << texture << endl;
    }
    // Step 2: Bones
    uint32_t subsection_offset = Binary::get_uint32(gxx, section_c + 0x8 + 2 * 0x8);
    uint32_t boneAmount = Binary::get_uint32(gxx, section_c + 0xC + 2 * 0x8);
        
    cout << endl << dye::black_on_light_green("Amount of bones:") << " " << boneAmount << endl;

    ///We go through each bone offset to get it's name
    for (int a = 0; a < boneAmount; a++)
    {
        ///first, we grab the offset to specific bone entry
        uint32_t entry_offset = subsection_offset + 0x8 * a;

        uint32_t bone_integer = Binary::get_uint32(gxx, entry_offset);
        uint32_t bone_name_pointer = Binary::get_uint32(gxx, entry_offset + 0x4);

        string bone_name = Binary::get_string(gxx, bone_name_pointer);

        cout << hue::light_green << "Bone " << a + 1 << ":" << hue::reset << " " << bone_name << ", unk_1: " << bone_integer << endl;
    }

    // Step 3: Mesh and Animations
    {
        uint32_t block_pointer = Binary::get_uint32(gxx, section_c + 0x10);
        uint32_t blocks = Binary::get_uint32(gxx, section_c + 0x14);

        cout << dye::black_on_light_aqua("Blocks start at:") << " 0x" << hex << block_pointer << dec << ", " << blocks << " blocks" << endl;
        cout << endl;

        for (int i = 0; i < blocks; i++)
        {
            uint32_t anim_pointer = Binary::get_uint32(gxx, block_pointer + (i * 0x10));
            uint32_t frames = Binary::get_uint32(gxx, block_pointer + (i * 0x10) + 0x4);
            float framerate = Binary::get_float(gxx, block_pointer + (i * 0x10) + 0x8);
            uint32_t frameloop = Binary::get_float(gxx, block_pointer + (i * 0x10) + 0xC);

            uint32_t anim_data = Binary::get_uint32(gxx, anim_pointer);
            uint32_t boneAmount = Binary::get_uint32(gxx, section_c + 0xC + 2 * 0x8);
            uint32_t anim_data_size = 0x0;
            anim_data_size = Binary::get_uint32(gxx, anim_data - 0x40 + 0x8);

            cout << dye::light_aqua("Anim ptr:") << " 0x" << std::hex << anim_pointer << " " << dye::light_aqua("Anim data:") << " 0x" << anim_data << std::dec << ", " << dye::light_aqua("Frames:") << " " << frames << " " << dye::light_aqua("Loop:") << " " << frameloop << " " << dye::light_aqua("Framerate:") << " " << framerate << endl;
            cout << dye::light_aqua("Anim data size:") << " 0x" << hex << anim_data_size << dec << endl;
            cout << endl;
            uint32_t gpu_offset = anim_data;

            uint32_t buffer = 0x0;
            buffer = Binary::get_uint32(gxx, gpu_offset);

            vector<uint32_t> unkownInstr;

            uint32_t vertexAddr;
            uint32_t indexAddr;
            vector<uint32_t> returnAddr;

            //cout << endl << "Amount of bones: " << boneAmount << endl;

            /// VERTEX TYPE
            string texcoord_type = "NULL";
            string color_type = "NULL";
            string position_type = "NULL";

            for (int f = 0; f < anim_data_size / 4 * frames; f++)
            {
                ///We go over the most crucial GPU instructions for us
                cout << hue::black_on_bright_white << "0x" << hex << gpu_offset << dec << hue::reset << dye::grey(" > ");
                //increasing gpu_offset before parsing instruction, so it can modifieed by instruction
                gpu_offset += 0x4;

                switch (buffer >> 24)
                {
                default:
                {
                    //if new instruction will be found, GXXTool will output list of new instructions after parsing finished
                    uint32_t data = buffer >> 24;
                    if (!std::count(unkownInstr.begin(), unkownInstr.end(), data))
                    {
                        unkownInstr.push_back(data);
                    }
                    break;
                }

                case GE_NOP:
                {
                    uint32_t data = buffer << 8 >> 8;
                    if (data != 0)
                    {
                        cout << hex << "NOP: data= 0x" << data << dec << endl;
                    }
                    else
                    {
                        cout << dye::grey("NOP") << endl;
                    }
                    break;
                }

                case GE_VADDR:
                {
                    uint32_t data = buffer << 8 >> 8;
                    if (data != 0)
                    {
                        vertexAddr = data;
                        cout << "VADDR: " << data << endl;
                    }
                    break;
                }

                case GE_IADDR:
                {
                    uint32_t data = buffer << 8 >> 8;
                    if (data != 0)
                    {
                        indexAddr = data;
                        cout << "IADDR: " << data << endl;
                    }
                    break;
                }

                case GE_PRIM:
                {
                    uint32_t count = buffer & 0xFFFF;
                    uint32_t type = (buffer >> 16) & 7;

                    static const char* types[8] =
                    {
                        "POINTS",
                        "LINES",
                        "LINE_STRIP",
                        "TRIANGLES",
                        "TRIANGLE_STRIP",
                        "TRIANGLE_FAN",
                        "RECTANGLES",
                        "CONTINUE_PREVIOUS",
                    };

                    cout << "DRAW PRIM \"" << (type < 7 ? types[type] : "INVALID") << "\" count= " << count << " vaddr= " << hex << vertexAddr << dec << endl;
                        
                    cout << dye::black_on_bright_white("  VERTEX DATA:") << endl;

                    int block_size = 0x00;
                    if (texcoord_type != "NULL")
                        block_size += 0x08;
                    if (color_type != "NULL")
                        block_size += 0x04;
                    if (position_type != "NULL")
                        block_size += 0x0C;

                    for (int i = 0; i < count; i++)
                    {
                        cout << "  ";

                        uint32_t offset = vertexAddr + block_size * i;

                        if (texcoord_type == "float")
                        {
                            float U = Binary::get_float(gxx, offset);
                            offset += 0x04;
                            float V = Binary::get_float(gxx, offset);
                            offset += 0x04;
                            cout << dye::light_yellow("UV:") << "(" << U << ", " << V << ") ";
                        }

                        if (color_type == "BGR 565")
                        {
                            uint16_t color = Binary::get_uint16(gxx, offset);
                            offset += 0x04;
                            cout << dye::light_purple("argb:") << "0x" << hex << color << " " << dec;
                        }
                        else if (color_type == "ABGR 8888")
                        {
                            uint32_t color = Binary::get_uint32(gxx, offset);
                            offset += 0x04;
                            cout << dye::light_purple("argb:") << "0x" << hex << color << " " << dec;
                        }

                        if (position_type == "float")
                        {
                            float x = Binary::get_float(gxx, offset);
                            offset += 0x04;
                            float y = Binary::get_float(gxx, offset);
                            offset += 0x04;
                            float z = Binary::get_float(gxx, offset);
                            offset += 0x04;

                            cout << "pos:(" << dye::light_red(x) << ", " << dye::light_green(y) << ", " << dye::light_aqua(z) << ") ";
                        }
                        //cout << "offset 0x" << hex << offset << dec;
                        cout << endl;
                    }
                        
                    vertexAddr += block_size * count;
                    break;
                }

                case GE_JUMP:
                {
                    uint32_t addr = buffer << 8 >> 8;
                    returnAddr.push_back(gpu_offset);
                    gpu_offset = addr;
                    cout << "Jump to: " << hex << "0x" << addr << dec << endl;
                    break;
                }
                
                case GE_CALL:
                {
                    uint32_t addr = buffer << 8 >> 8;
                    returnAddr.push_back(gpu_offset);
                    gpu_offset = addr;
                    cout << "Call to: " << hex << "0x" << addr << dec << endl;
                    break;
                }
                
                case GE_RET:
                {
                    if (returnAddr.size() > 0 )
                    {
                        uint32_t addr = returnAddr.back();
                        returnAddr.pop_back();
                        gpu_offset = addr;
                        cout << "Return to: " << hex << "0x" << addr << dec << endl;
                    }
                    else
                    {
                        cout << "Nowhere to return" << endl;
                    }


                    break;
                }

                case GE_BASE:
                {
                    uint32_t addr = buffer << 8 >> 8;
                    cout << "Set base addr to: " << hex << "0x" << addr << dec << endl;
                    break;
                }

                case GE_VERTEXTYPE:
                {
                    ///Clear vertex type
                    texcoord_type = "NULL";
                    color_type = "NULL";
                    position_type = "NULL";


                    uint32_t op = buffer;
                    cout << "SetVertexType: ";

                    bool through = (op & GE_VTYPE_THROUGH_MASK) == GE_VTYPE_THROUGH;
                    int tc = (op & GE_VTYPE_TC_MASK) >> GE_VTYPE_TC_SHIFT;
                    int col = (op & GE_VTYPE_COL_MASK) >> GE_VTYPE_COL_SHIFT;
                    int nrm = (op & GE_VTYPE_NRM_MASK) >> GE_VTYPE_NRM_SHIFT;
                    int pos = (op & GE_VTYPE_POS_MASK) >> GE_VTYPE_POS_SHIFT;
                    int weight = (op & GE_VTYPE_WEIGHT_MASK) >> GE_VTYPE_WEIGHT_SHIFT;
                    int weightCount = ((op & GE_VTYPE_WEIGHTCOUNT_MASK) >> GE_VTYPE_WEIGHTCOUNT_SHIFT) + 1;
                    int morphCount = (op & GE_VTYPE_MORPHCOUNT_MASK) >> GE_VTYPE_MORPHCOUNT_SHIFT;
                    int idx = (op & GE_VTYPE_IDX_MASK) >> GE_VTYPE_IDX_SHIFT;

                    string colorNames[] = {
                        "NULL",
                        "unsupported1",
                        "unsupported2",
                        "unsupported3",
                        "BGR 565",
                        "ABGR 1555",
                        "ABGR 4444",
                        "ABGR 8888",
                    };
                    string typeNames[] = {
                        "NULL",
                        "u8",
                        "u16",
                        "float",
                    };
                    string typeNamesI[] = {
                        "NULL",
                        "u8",
                        "u16",
                        "u32",
                    };
                    string typeNamesS[] = {
                        "NULL",
                        "s8",
                        "s16",
                        "float",
                    };

                    if (through)
                        cout << "through, ";
                    if (typeNames[tc] != "NULL")
                        cout << typeNames[tc] << " texcoords, ";
                        texcoord_type = typeNames[tc];

                    if (colorNames[col] != "NULL")
                        cout << colorNames[col] << " colors, ";
                        color_type = colorNames[col];

                    if (typeNames[nrm] != "NULL")
                        cout << typeNamesS[nrm] << " normals, ";

                    if (typeNames[pos] != "NULL")
                        cout << typeNamesS[pos] << " positions, ";
                        position_type = typeNamesS[pos];

                    if (typeNames[weight] != "NULL")
                        cout << typeNames[weight] << " weights " << weightCount << ", ";
                    else if (weightCount > 1)
                        cout << "unknown weights " << weightCount << ", ";

                    if (morphCount > 0)
                        cout << morphCount << " morphs, ";

                    if (typeNamesI[idx] != "NULL")
                        cout << typeNamesI[idx] << " indexes";

                    cout << endl;

                        

                    break;
                }

                case GE_TEXTUREMAPENABLE:
                {
                    uint32_t flag = buffer << 8 >> 8;
                    cout << "Texture map enable: " << hex << flag << dec << endl;
                    break;
                }

                case GE_WORLDMATRIXNUMBER:
                {
                    /* PPSSPP Source
                    if (data & ~0xF)
                        snprintf(buffer, bufsize, "World # %i (extra %x)", data & 0xF, data);
                    else
                        snprintf(buffer, bufsize, "World # %i", data & 0xF);
                    */
                    int f = buffer << 8;

                    //cout.setf(ios::fixed, ios::floatfield);
                    //cout.setf(ios::showpoint);
                    cout << "World: # " << (int)Binary::u32_to_float(f) << endl;
                    cout << dye::black_on_bright_white("\tBONE DATA:") << endl;

                    BoneMatrix matrix;

                    matrix.basisX_x = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset) << 8);
                    matrix.basisX_y = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x04) << 8);
                    matrix.basisX_z = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x08) << 8);
                        
                    matrix.basisY_x = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x0c) << 8);
                    matrix.basisY_y = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x10) << 8);
                    matrix.basisY_z = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x14) << 8);
                        
                    matrix.basisZ_x = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x18) << 8);
                    matrix.basisZ_y = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x1C) << 8);
                    matrix.basisZ_z = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x20) << 8);

                    matrix.translation_x = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x24) << 8);
                    matrix.translation_y = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x28) << 8);
                    matrix.translation_z = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x2C) << 8);


                    cout << "\tPosition: (" << dye::light_red(matrix.translation_x) << ", " <<              dye::light_green(matrix.translation_y) << ", " <<               dye::light_aqua(matrix.translation_z) << ") " << endl;
                    cout << "\tScale:    (" << dye::light_red(matrix.get_scale_x()) << ", " <<              dye::light_green(matrix.get_scale_y()) << ", " <<               dye::light_aqua(matrix.get_scale_z()) << ") " << endl;
                    cout << "\tRadians:  (" << dye::light_red(matrix.get_rotation().x) << ", " <<           dye::light_green(matrix.get_rotation().y) << ", " <<            dye::light_aqua(matrix.get_rotation().z) << ") " << endl;
                    cout << "\tDegrees:  (" << dye::light_red(rad2deg(matrix.get_rotation().x)) << ", " <<  dye::light_green(rad2deg(matrix.get_rotation().y)) << ", " <<   dye::light_aqua(rad2deg(matrix.get_rotation().z)) << ") " << endl;
                    break;
                }

                case GE_WORLDMATRIXDATA:
                {
                    uint32_t data = buffer << 8;
                    float f = Binary::u32_to_float(data);

                    cout.setf(ios::fixed, ios::floatfield);
                    cout.setf(ios::showpoint);
                    cout << "World matrix set: " << f << endl;

                    break;
                }

                case GE_TEXSCALEU:
                {
                    uint32_t f = buffer << 8;

                    cout.setf(ios::fixed, ios::floatfield);
                    cout.setf(ios::showpoint);
                    cout << "Texture U scale: " << Binary::u32_to_float(f) << endl;

                    break;
                }

                case GE_TEXSCALEV:
                {
                    uint32_t f = buffer << 8;

                    cout.setf(ios::fixed, ios::floatfield);
                    cout.setf(ios::showpoint);
                    cout << "Texture V scale: " << Binary::u32_to_float(f) << endl;

                    break;
                }

                case GE_TEXOFFSETU:
                {
                    uint32_t f = buffer << 8;

                    cout.setf(ios::fixed, ios::floatfield);
                    cout.setf(ios::showpoint);
                    cout << "Texture U offset: " << Binary::u32_to_float(f) << endl;

                    break;
                }

                case GE_TEXOFFSETV:
                {
                    uint32_t f = buffer << 8;

                    cout.setf(ios::fixed, ios::floatfield);
                    cout.setf(ios::showpoint);
                    cout << "Texture V offset: " << Binary::u32_to_float(f) << endl;

                    break;
                }

                case GE_MATERIALAMBIENT:
                {
                    uint32_t color = buffer << 8 >> 8;
                    cout << "Material ambient color: 0x" << hex << color << dec << endl;
                    break;
                }
                
                case GE_MATERIALALPHA:
                {
                    uint32_t color = buffer << 8 >> 8;

                    cout.setf(ios::fixed, ios::floatfield);
                    cout.setf(ios::showpoint);
                    cout << "Material alpha color: 0x" << hex << color << dec << endl;

                    break;
                }

                }

                if (returnAddr.size() > 0)
                {
                    ///if GE perform ump or call, we should not count until it returns to animation data block
                    f--;
                }
                buffer = Binary::get_uint32(gxx, gpu_offset);
            }

            cout << endl;

            for (uint32_t instr : unkownInstr)
            {
                //output list of new found instructions
                cout << "Unknown istruction found: 0x" << hex << instr << dec << endl;
            }

            cout << endl;
            
        }


    }

    system("pause");
    return 0;
}

uint32_t bgr_to_rgba(uint16_t bgr565)
{
    /// bgr_to_rgba by Owocek
    
    ///bgr565
    uint8_t b = bgr565 >> 11;
    uint8_t g = uint16_t(bgr565 << 5) >> 10;
    uint8_t r = uint16_t(bgr565 << 11) >> 11;
    
    ///rgb888
    uint8_t red = (r * 527 + 23) >> 6;
    uint8_t green = (g * 259 + 33) >> 6;
    uint8_t blue = (b * 527 + 23) >> 6;

    return (uint32_t(red) << 24) + (uint32_t(green) << 16) + (uint32_t(blue) << 8) + 0xFF;
}

float rad2deg(float angle) {
    return angle * 180.0 / Math_PI;
}
