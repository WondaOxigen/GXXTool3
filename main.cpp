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
    cout << "GXXTool3 by Owocek and wondaoxigen" << endl << endl;

    if (argc < 2) 
    {
        // Wonda: checking if we have a file 
        std::cout << "No file specified.\n";
        system("pause");
        return 0;
    }

    //string file = "chr01_01_01_1.gxx";
    string file = argv[1];
    cout << "Reading file " << file << endl << endl;

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

    debug_log("get main offsets");
    uint32_t main_offsets = Binary::get_uint32(gxx, section_c);

    debug_log("start loop");
    {
        //for (uint32_t i = 0; i < main_offsets; i++)
        //{
        //    uint32_t subsection_offset = Binary::get_uint32(gxx, section_c + 0x8 + i * 0x8);
        //    uint32_t subsection_size = Binary::get_uint32(gxx, section_c + 0xC + i * 0x8);
        //    debug_log("subsection data");

        //    ///Here we switch through i because main offset 1, 2 and 3 are a bit different from each other
        //    ///and we wanna make sure we don't load incorrect data
        //    switch (i)
        //    {
        //    case 0: ///we start from the first offset
        //    {
        //        debug_log("case 1");
        //        ///first offset only contains one offset in itself, so we store it
        //        ///the offset leads to section D which holds info about loaded texture
        //        uint32_t section_d = Binary::get_uint32(gxx, subsection_offset);

        //        ///We grab a pointer to the texture name
        //        uint32_t name_pointer = Binary::get_uint32(gxx, section_d + 0x4);

        //        ///We can grab texture name now
        //        string name = Binary::get_string(gxx, name_pointer);

        //        cout << "GXX texture name: " << name << endl << endl;

        //        ///And thats it for the first offset.
        //        break;
        //    }

        //    case 1: ///second offset is more interesting. It contains data of graphical instruction blocks
        //    {
        //        debug_log("case 2");
        //        cout << "Reading GPU instruction offsets:" << endl;

        //        uint32_t all_gpus = 0;

        //        ///However, it first starts with a jump table. We gotta go through it
        //        for (uint32_t a = 0; a < subsection_size; a++)
        //        {
        //            ///first, we grab the offset to jump table entry
        //            uint32_t jt_offset = subsection_offset + 0x10 * a;

        //            ///then, we can start grabbing the gpu offsets
        //            uint32_t gpu_start_offset = Binary::get_uint32(gxx, jt_offset);

        //            ///and we load some additional data from the jump table
        //            uint32_t gpu_offset_size = Binary::get_uint32(gxx, jt_offset + 0x4); ///Amount of gpu offsets
        //            float gpu_float = Binary::get_float(gxx, jt_offset + 0x8); ///Some unknown float

        //            ///And now we can loop through the actual gpu offsets
        //            for (int b = 0; b < gpu_offset_size; b++)
        //            {
        //                ///Here we read each GPU offset one by one. We can probably grab some useful data from here
        //                uint32_t gpu_offset = Binary::get_uint32(gxx, gpu_start_offset + 0x4 * b);
        //                cout << "Subsection " << a + 1 << ", GPU offset " << b + 1 << ": " << hex << "0x" << gpu_offset << dec << endl;

        //                all_gpus++;

        //                ///And we can go ahead and extract the GPU instruction data
        //                uint32_t buffer = 0x0;
        //                buffer = Binary::get_uint32(gxx, gpu_offset);

        //                while (buffer != 0x0B000000)
        //                {
        //                    ///We go over the most crucial GPU instructions for us
        //                    switch (buffer >> 24)
        //                    {
        //                    case GE_NOP:
        //                    {
        //                        uint32_t data = buffer << 8 >> 8;
        //                        if (data != 0)
        //                        {
        //                            cout << "NOP: data= " << data << endl;
        //                        }
        //                        else
        //                        {
        //                            cout << "NOP" << endl;
        //                        }
        //                        break;
        //                    }

        //                    case GE_VADDR:
        //                    {
        //                        uint32_t data = buffer << 8 >> 8;
        //                        if (data != 0)
        //                        {
        //                            cout << "VADDR: " << data << endl;
        //                        }
        //                        break;
        //                    }

        //                    case GE_IADDR:
        //                    {
        //                        uint32_t data = buffer << 8 >> 8;
        //                        if (data != 0)
        //                        {
        //                            cout << "IADDR: " << data << endl;
        //                        }
        //                        break;
        //                    }

        //                    case GE_PRIM:
        //                    {
        //                        uint32_t count = buffer & 0xFFFF;
        //                        uint32_t type = (buffer >> 16) & 7;

        //                        static const char* types[8] =
        //                        {
        //                            "POINTS",
        //                            "LINES",
        //                            "LINE_STRIP",
        //                            "TRIANGLES",
        //                            "TRIANGLE_STRIP",
        //                            "TRIANGLE_FAN",
        //                            "RECTANGLES",
        //                            "CONTINUE_PREVIOUS",
        //                        };

        //                        cout << "DRAW PRIM %s: count= " << (type < 7 ? types[type] : "INVALID") << "vaddr= " << "TODO" << endl;

        //                        break;
        //                    }

        //                    case GE_CALL:
        //                    {
        //                        uint32_t addr = buffer << 8 >> 8;
        //                        cout << "Jump (call) to: " << hex << "0x" << addr << dec << endl;
        //                        break;
        //                    }

        //                    case GE_BASE:
        //                    {
        //                        uint32_t addr = buffer << 8 >> 8;
        //                        cout << "Set base addr to: " << hex << "0x" << addr << dec << endl;
        //                        break;
        //                    }

        //                    case GE_WORLDMATRIXNUMBER:
        //                    {
        //                        /* PPSSPP Source
        //                        if (data & ~0xF)
        //                            snprintf(buffer, bufsize, "World # %i (extra %x)", data & 0xF, data);
        //                        else
        //                            snprintf(buffer, bufsize, "World # %i", data & 0xF);
        //                        */
        //                        uint32_t f = buffer << 8;

        //                        cout.setf(ios::fixed, ios::floatfield);
        //                        cout.setf(ios::showpoint);
        //                        cout << "World: # " << Binary::u32_to_float(f) << endl;

        //                        break;
        //                    }

        //                    case GE_WORLDMATRIXDATA:
        //                    {
        //                        uint32_t f = buffer << 8;

        //                        cout.setf(ios::fixed, ios::floatfield);
        //                        cout.setf(ios::showpoint);
        //                        cout << "World matrix set: " << Binary::u32_to_float(f) << endl;

        //                        break;
        //                    }

        //                    case GE_TEXSCALEU:
        //                    {
        //                        uint32_t f = buffer << 8;

        //                        cout.setf(ios::fixed, ios::floatfield);
        //                        cout.setf(ios::showpoint);
        //                        cout << "Texture U scale: " << Binary::u32_to_float(f) << endl;

        //                        break;
        //                    }

        //                    case GE_TEXSCALEV:
        //                    {
        //                        uint32_t f = buffer << 8;

        //                        cout.setf(ios::fixed, ios::floatfield);
        //                        cout.setf(ios::showpoint);
        //                        cout << "Texture V scale: " << Binary::u32_to_float(f) << endl;

        //                        break;
        //                    }

        //                    case GE_TEXOFFSETU:
        //                    {
        //                        uint32_t f = buffer << 8;

        //                        cout.setf(ios::fixed, ios::floatfield);
        //                        cout.setf(ios::showpoint);
        //                        cout << "Texture U offset: " << Binary::u32_to_float(f) << endl;

        //                        break;
        //                    }

        //                    case GE_TEXOFFSETV:
        //                    {
        //                        uint32_t f = buffer << 8;

        //                        cout.setf(ios::fixed, ios::floatfield);
        //                        cout.setf(ios::showpoint);
        //                        cout << "Texture V offset: " << Binary::u32_to_float(f) << endl;

        //                        break;
        //                    }

        //                    case GE_MATERIALALPHA:
        //                    {
        //                        uint32_t f = buffer << 8;

        //                        cout.setf(ios::fixed, ios::floatfield);
        //                        cout.setf(ios::showpoint);
        //                        cout << "Material alpha color: " << Binary::u32_to_float(f) << endl;

        //                        break;
        //                    }

        //                    }

        //                    gpu_offset += 0x4;
        //                    buffer = Binary::get_uint32(gxx, gpu_offset);
        //                }
        //            }
        //        }

        //        cout << "All GPU instruction blocks accessed: " << dec << all_gpus << endl;

        //        break;
        //    }

        //    case 2: ///third offset contains information about limbs
        //    {
        //        debug_log("case 3");
        //        cout << endl << "Amount of bones: " << subsection_size << endl;

        //        ///We go through each bone offset to get it's name
        //        for (int a = 0; a < subsection_size; a++)
        //        {
        //            ///first, we grab the offset to specific bone entry
        //            uint32_t entry_offset = subsection_offset + 0x8 * a;

        //            uint32_t bone_integer = Binary::get_uint32(gxx, entry_offset);
        //            uint32_t bone_name_pointer = Binary::get_uint32(gxx, entry_offset + 0x4);

        //            string bone_name = Binary::get_string(gxx, bone_name_pointer);

        //            cout << "Bone " << a + 1 << ": " << bone_name << ", unk_1: " << bone_integer << endl;
        //        }

        //        break;
        //    }
        //    }
        //}
    }

    // Step 1: Texture name
    string texture;
    {
        uint32_t offset_1 = Binary::get_uint32(gxx, section_c + 0x08);
        uint32_t offset_2 = Binary::get_uint32(gxx, offset_1);
        uint32_t offset_3 = Binary::get_uint32(gxx, offset_2 + 0x04);
        texture = Binary::get_string(gxx, offset_3);
        cout << "Texture: " << texture << endl;
    }
    // Step 2: Bones
    uint32_t subsection_offset = Binary::get_uint32(gxx, section_c + 0x8 + 2 * 0x8);
    uint32_t boneAmount = Binary::get_uint32(gxx, section_c + 0xC + 2 * 0x8);
        
    cout << endl << "Amount of bones: " << boneAmount << endl;

    ///We go through each bone offset to get it's name
    for (int a = 0; a < boneAmount; a++)
    {
        ///first, we grab the offset to specific bone entry
        uint32_t entry_offset = subsection_offset + 0x8 * a;

        uint32_t bone_integer = Binary::get_uint32(gxx, entry_offset);
        uint32_t bone_name_pointer = Binary::get_uint32(gxx, entry_offset + 0x4);

        string bone_name = Binary::get_string(gxx, bone_name_pointer);

        cout << "Bone " << a + 1 << ": " << bone_name << ", unk_1: " << bone_integer << endl;
    }

    // Step 3: Mesh and Animations
    {
        uint32_t block_pointer = Binary::get_uint32(gxx, section_c + 0x10);
        uint32_t blocks = Binary::get_uint32(gxx, section_c + 0x14);

        cout << "Blocks start at: 0x" << hex << block_pointer << dec << ", " << blocks << " blocks" << endl;

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

            //for (int i = 0; i < boneAmount; i++)
            //{ 
            cout << "Anim ptr: 0x" << std::hex << anim_pointer << " (anim data at: 0x" << anim_data << std::dec << "), Frames: " << frames << " Loop: " << frameloop << " Framerate: " << framerate << endl;
            cout << "Anim data size: 0x" << hex << anim_data_size << dec << endl;

            //bool end = false;
            uint32_t gpu_offset = anim_data;

            uint32_t buffer = 0x0;
            buffer = Binary::get_uint32(gxx, gpu_offset);

            vector<uint32_t> unkownInstr;

            uint32_t vertexAddr;
            uint32_t indexAddr;
            vector<uint32_t> returnAddr;

            cout << endl << "Amount of bones: " << boneAmount << endl;

            /// VERTEX TYPE
            string texcoord_type = "NULL";
            string color_type = "NULL";
            string position_type = "NULL";

            //while (!end)
            //{
            for (int f = 0; f < anim_data_size / 4 * frames; f++)
            {
                ///We go over the most crucial GPU instructions for us
                cout << dye::light_red_on_bright_white("f: ") << dye::light_red_on_bright_white(f) << dye::light_red_on_bright_white(" size: ") << dye::light_red_on_bright_white(anim_data_size / 4 * frames) << " ";
                cout << dye::black_on_bright_white("0x")  << hex << dye::black_on_bright_white(gpu_offset) << dec << " ";
                gpu_offset += 0x4;



                switch (buffer >> 24)
                {
                default:
                {
                    uint32_t data = buffer >> 24;
                    //cout << "---UNKNOWN: " << hex << "0x" << data << dec << endl;
                    if (!std::count(unkownInstr.begin(), unkownInstr.end(), data))
                    {
                        unkownInstr.push_back(data);
                    }
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
                        cout << "NOP" << endl;
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
                        
                    cout << "VERTEX DATA:" << endl;

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
                            cout << "UV:(" << U << ", " << V << ") ";
                        }

                        if (color_type == "BGR 565")
                        {
                            uint16_t color = Binary::get_uint16(gxx, offset);
                            offset += 0x04;
                            cout << "color:0x" << hex << color << " ";
                        }
                        else if (color_type == "ABGR 8888")
                        {
                            uint32_t color = Binary::get_uint32(gxx, offset);
                            offset += 0x04;
                            cout << "color:0x" << hex << color << " ";
                        }

                        if (position_type == "float")
                        {
                            float x = Binary::get_float(gxx, offset);
                            offset += 0x04;
                            float y = Binary::get_float(gxx, offset);
                            offset += 0x04;
                            float z = Binary::get_float(gxx, offset);
                            offset += 0x04;

                            cout << "xyz:(" << x << ", " << y << ", " << z << ") ";
                        }
                        //cout << "offset 0x" << hex << offset;
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
                    //else
                    //{
                    //    end = true;
                    //}


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

                    cout.setf(ios::fixed, ios::floatfield);
                    cout.setf(ios::showpoint);
                    cout << "World: # " << (int)Binary::u32_to_float(f) << endl;

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

                    //matrix.basisX_x = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset) << 8);
                    //matrix.basisY_x = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x04) << 8);
                    //matrix.basisZ_x = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x08) << 8);

                    //matrix.basisX_y = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x0c) << 8);
                    //matrix.basisY_y = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x10) << 8);
                    //matrix.basisZ_y = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x14) << 8);

                    //matrix.basisX_z = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x18) << 8);
                    //matrix.basisY_z = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x1C) << 8);
                    //matrix.basisZ_z = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x20) << 8);

                    matrix.translation_x = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x24) << 8);
                    matrix.translation_y = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x28) << 8);
                    matrix.translation_z = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x2C) << 8);

                    /*cout << endl;
                    cout << "matrix 1: " << hex << Binary::get_uint32(gxx, gpu_offset + 0x04) << dec << 8 << endl;
                    cout << "matrix 2: " <<  hex << Binary::get_uint32(gxx, gpu_offset + 0x08) << dec  << 8 << endl;
                    cout << "matrix 3: " <<  hex << Binary::get_uint32(gxx, gpu_offset + 0x0c) << dec  << 8 << endl;
                                           
                    cout << "matrix 4: " <<  hex << Binary::get_uint32(gxx, gpu_offset + 0x10) << dec  << 8 << endl;
                    cout << "matrix 5: " <<  hex << Binary::get_uint32(gxx, gpu_offset + 0x14) << dec  << 8 << endl;
                    cout << "matrix 6: " <<  hex << Binary::get_uint32(gxx, gpu_offset + 0x18) << dec  << 8 << endl;
                                           
                    cout << "matrix 7: " <<  hex << Binary::get_uint32(gxx, gpu_offset + 0x1C) << dec  << 8 << endl;
                    cout << "matrix 8: " <<  hex << Binary::get_uint32(gxx, gpu_offset + 0x20) << dec  << 8 << endl;
                    cout << "matrix 9: " <<  hex << Binary::get_uint32(gxx, gpu_offset + 0x24) << dec  << 8 << endl;
                        
                    cout << "matrix 10: " <<  hex << Binary::get_uint32(gxx, gpu_offset + 0x28) << dec  << 8 << endl;
                    cout << "matrix 11: " <<  hex << Binary::get_uint32(gxx, gpu_offset + 0x2C) << dec  << 8 << endl;
                    cout << "matrix 12: " <<  hex << Binary::get_uint32(gxx, gpu_offset + 0x30) << dec  << 8 << endl;
                    cout << endl;*/


                    cout << "xyz: (" << matrix.translation_x << ", " << matrix.translation_y << ", " << matrix.translation_z << ") " << endl;
                    cout << "Scale xyz: (" << matrix.get_scale_x() << ", " << matrix.get_scale_y() << ", " << matrix.get_scale_z() << ") " << endl;
                    cout << "    12" << endl;
                    cout << "Rotation rad xyz: (" << matrix.get_rotation().x << ", " << matrix.get_rotation().y << ", " << matrix.get_rotation().z << ") " << endl;
                    cout << "Rotation deg xyz: (" << rad2deg(matrix.get_rotation().x) << ", " << rad2deg(matrix.get_rotation().y) << ", " << rad2deg(matrix.get_rotation().z) << ") " << endl;
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
                    f--;
                }
                buffer = Binary::get_uint32(gxx, gpu_offset);
            }

            cout << endl;

            for (uint32_t instr : unkownInstr)
            {
                cout << "Unknown istruction found: 0x" << hex << instr << dec << endl;
            }

            cout << endl;
            //} for
            
            {}
            {
                //for (int i = 0; i < boneAmount; i++)
                //{
                //    cout << "Anim ptr: 0x" << std::hex << anim_pointer << " (anim data at: 0x" << anim_data << std::dec << "), Frames: " << frames << " Loop: " << frameloop << " Framerate: " << framerate << endl;

                //    bool end = false;
                //    uint32_t gpu_offset = anim_data;

                //    uint32_t buffer = 0x0;
                //    buffer = Binary::get_uint32(gxx, gpu_offset);

                //    vector<uint32_t> unkownInstr;

                //    uint32_t vertexAddr;
                //    uint32_t indexAddr;
                //    vector<uint32_t> returnAddr;

                //    cout << endl << "Amount of bones: " << subsection_size << endl;

                //    /// VERTEX TYPE
                //    string texcoord_type = "NULL";
                //    string color_type = "NULL";
                //    string position_type = "NULL";

                //    while (!end)
                //    {
                //        ///We go over the most crucial GPU instructions for us
                //        gpu_offset += 0x4;

                //        switch (buffer >> 24)
                //        {
                //        default:
                //        {
                //            uint32_t data = buffer >> 24;
                //            //cout << "---UNKNOWN: " << hex << "0x" << data << dec << endl;
                //            if (!std::count(unkownInstr.begin(), unkownInstr.end(), data))
                //            {
                //                unkownInstr.push_back(data);
                //            }
                //        }

                //        case GE_NOP:
                //        {
                //            uint32_t data = buffer << 8 >> 8;
                //            if (data != 0)
                //            {
                //                cout << hex << "NOP: data= 0x" << data << dec << endl;
                //            }
                //            else
                //            {
                //                cout << "NOP" << endl;
                //            }
                //            break;
                //        }

                //        case GE_VADDR:
                //        {
                //            uint32_t data = buffer << 8 >> 8;
                //            if (data != 0)
                //            {
                //                vertexAddr = data;
                //                cout << "VADDR: " << data << endl;
                //            }
                //            break;
                //        }

                //        case GE_IADDR:
                //        {
                //            uint32_t data = buffer << 8 >> 8;
                //            if (data != 0)
                //            {
                //                indexAddr = data;
                //                cout << "IADDR: " << data << endl;
                //            }
                //            break;
                //        }

                //        case GE_PRIM:
                //        {
                //            uint32_t count = buffer & 0xFFFF;
                //            uint32_t type = (buffer >> 16) & 7;

                //            static const char* types[8] =
                //            {
                //                "POINTS",
                //                "LINES",
                //                "LINE_STRIP",
                //                "TRIANGLES",
                //                "TRIANGLE_STRIP",
                //                "TRIANGLE_FAN",
                //                "RECTANGLES",
                //                "CONTINUE_PREVIOUS",
                //            };

                //            cout << "DRAW PRIM \"" << (type < 7 ? types[type] : "INVALID") << "\" count= " << count << " vaddr= " << hex << vertexAddr << dec << endl;

                //            cout << "VERTEX DATA:" << endl;

                //            int block_size = 0x00;
                //            if (texcoord_type != "NULL")
                //                block_size += 0x08;
                //            if (color_type != "NULL")
                //                block_size += 0x04;
                //            if (position_type != "NULL")
                //                block_size += 0x0C;

                //            for (int i = 0; i < count; i++)
                //            {
                //                cout << "  ";

                //                uint32_t offset = vertexAddr + block_size * i;

                //                if (texcoord_type == "float")
                //                {
                //                    float U = Binary::get_float(gxx, offset);
                //                    offset += 0x04;
                //                    float V = Binary::get_float(gxx, offset);
                //                    offset += 0x04;
                //                    cout << "UV:(" << U << ", " << V << ") ";
                //                }

                //                if (color_type == "BGR 565")
                //                {
                //                    uint16_t color = Binary::get_uint16(gxx, offset);
                //                    offset += 0x04;
                //                    cout << "color:0x" << hex << color << " ";
                //                }
                //                else if (color_type == "ABGR 8888")
                //                {
                //                    uint32_t color = Binary::get_uint32(gxx, offset);
                //                    offset += 0x04;
                //                    cout << "color:0x" << hex << color << " ";
                //                }

                //                if (position_type == "float")
                //                {
                //                    float x = Binary::get_float(gxx, offset);
                //                    offset += 0x04;
                //                    float y = Binary::get_float(gxx, offset);
                //                    offset += 0x04;
                //                    float z = Binary::get_float(gxx, offset);
                //                    offset += 0x04;

                //                    cout << "xyz:(" << x << ", " << y << ", " << z << ") ";
                //                }
                //                //cout << "offset 0x" << hex << offset;
                //                cout << endl;
                //            }

                //            vertexAddr += block_size * count;
                //            break;
                //        }

                //        case GE_JUMP:
                //        {
                //            uint32_t addr = buffer << 8 >> 8;
                //            returnAddr.push_back(gpu_offset);
                //            gpu_offset = addr;
                //            cout << "Jump to: " << hex << "0x" << addr << dec << endl;
                //            break;
                //        }
                //        case GE_CALL:
                //        {
                //            uint32_t addr = buffer << 8 >> 8;
                //            returnAddr.push_back(gpu_offset);
                //            gpu_offset = addr;
                //            cout << "Call to: " << hex << "0x" << addr << dec << endl;
                //            break;
                //        }
                //        case GE_RET:
                //        {
                //            if (returnAddr.size() > 0)
                //            {
                //                uint32_t addr = returnAddr.back();
                //                returnAddr.pop_back();
                //                gpu_offset = addr;
                //                cout << "Return to: " << hex << "0x" << addr << dec << endl;
                //            }
                //            else
                //            {
                //                end = true;
                //            }


                //            break;
                //        }

                //        case GE_BASE:
                //        {
                //            uint32_t addr = buffer << 8 >> 8;
                //            cout << "Set base addr to: " << hex << "0x" << addr << dec << endl;
                //            break;
                //        }

                //        case GE_VERTEXTYPE:
                //        {
                //            ///Clear vertex type
                //            texcoord_type = "NULL";
                //            color_type = "NULL";
                //            position_type = "NULL";


                //            uint32_t op = buffer;
                //            cout << "SetVertexType: ";

                //            bool through = (op & GE_VTYPE_THROUGH_MASK) == GE_VTYPE_THROUGH;
                //            int tc = (op & GE_VTYPE_TC_MASK) >> GE_VTYPE_TC_SHIFT;
                //            int col = (op & GE_VTYPE_COL_MASK) >> GE_VTYPE_COL_SHIFT;
                //            int nrm = (op & GE_VTYPE_NRM_MASK) >> GE_VTYPE_NRM_SHIFT;
                //            int pos = (op & GE_VTYPE_POS_MASK) >> GE_VTYPE_POS_SHIFT;
                //            int weight = (op & GE_VTYPE_WEIGHT_MASK) >> GE_VTYPE_WEIGHT_SHIFT;
                //            int weightCount = ((op & GE_VTYPE_WEIGHTCOUNT_MASK) >> GE_VTYPE_WEIGHTCOUNT_SHIFT) + 1;
                //            int morphCount = (op & GE_VTYPE_MORPHCOUNT_MASK) >> GE_VTYPE_MORPHCOUNT_SHIFT;
                //            int idx = (op & GE_VTYPE_IDX_MASK) >> GE_VTYPE_IDX_SHIFT;

                //            string colorNames[] = {
                //                "NULL",
                //                "unsupported1",
                //                "unsupported2",
                //                "unsupported3",
                //                "BGR 565",
                //                "ABGR 1555",
                //                "ABGR 4444",
                //                "ABGR 8888",
                //            };
                //            string typeNames[] = {
                //                "NULL",
                //                "u8",
                //                "u16",
                //                "float",
                //            };
                //            string typeNamesI[] = {
                //                "NULL",
                //                "u8",
                //                "u16",
                //                "u32",
                //            };
                //            string typeNamesS[] = {
                //                "NULL",
                //                "s8",
                //                "s16",
                //                "float",
                //            };

                //            if (through)
                //                cout << "through, ";
                //            if (typeNames[tc] != "NULL")
                //                cout << typeNames[tc] << " texcoords, ";
                //            texcoord_type = typeNames[tc];

                //            if (colorNames[col] != "NULL")
                //                cout << colorNames[col] << " colors, ";
                //            color_type = colorNames[col];

                //            if (typeNames[nrm] != "NULL")
                //                cout << typeNamesS[nrm] << " normals, ";

                //            if (typeNames[pos] != "NULL")
                //                cout << typeNamesS[pos] << " positions, ";
                //            position_type = typeNamesS[pos];

                //            if (typeNames[weight] != "NULL")
                //                cout << typeNames[weight] << " weights " << weightCount << ", ";
                //            else if (weightCount > 1)
                //                cout << "unknown weights " << weightCount << ", ";

                //            if (morphCount > 0)
                //                cout << morphCount << " morphs, ";

                //            if (typeNamesI[idx] != "NULL")
                //                cout << typeNamesI[idx] << " indexes";

                //            cout << endl;



                //            break;
                //        }

                //        case GE_TEXTUREMAPENABLE:
                //        {
                //            uint32_t flag = buffer << 8 >> 8;
                //            cout << "Texture map enable: " << hex << flag << dec << endl;
                //            break;
                //        }

                //        case GE_WORLDMATRIXNUMBER:
                //        {
                //            /* PPSSPP Source
                //            if (data & ~0xF)
                //                snprintf(buffer, bufsize, "World # %i (extra %x)", data & 0xF, data);
                //            else
                //                snprintf(buffer, bufsize, "World # %i", data & 0xF);
                //            */
                //            int f = buffer << 8;

                //            cout.setf(ios::fixed, ios::floatfield);
                //            cout.setf(ios::showpoint);
                //            cout << "World: # " << (int)Binary::u32_to_float(f) << endl;

                //            BoneMatrix matrix;

                //            matrix.basisX_x = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset) << 8);
                //            matrix.basisX_y = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x04) << 8);
                //            matrix.basisX_z = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x08) << 8);

                //            matrix.basisY_x = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x0c) << 8);
                //            matrix.basisY_y = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x10) << 8);
                //            matrix.basisY_z = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x14) << 8);

                //            matrix.basisZ_x = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x18) << 8);
                //            matrix.basisZ_y = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x1C) << 8);
                //            matrix.basisZ_z = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x20) << 8);

                //            //matrix.basisX_x = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset) << 8);
                //            //matrix.basisY_x = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x04) << 8);
                //            //matrix.basisZ_x = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x08) << 8);

                //            //matrix.basisX_y = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x0c) << 8);
                //            //matrix.basisY_y = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x10) << 8);
                //            //matrix.basisZ_y = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x14) << 8);

                //            //matrix.basisX_z = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x18) << 8);
                //            //matrix.basisY_z = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x1C) << 8);
                //            //matrix.basisZ_z = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x20) << 8);

                //            matrix.translation_x = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x24) << 8);
                //            matrix.translation_y = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x28) << 8);
                //            matrix.translation_z = Binary::u32_to_float(Binary::get_uint32(gxx, gpu_offset + 0x2C) << 8);

                //            /*cout << endl;
                //            cout << "matrix 1: " << hex << Binary::get_uint32(gxx, gpu_offset + 0x04) << dec << 8 << endl;
                //            cout << "matrix 2: " <<  hex << Binary::get_uint32(gxx, gpu_offset + 0x08) << dec  << 8 << endl;
                //            cout << "matrix 3: " <<  hex << Binary::get_uint32(gxx, gpu_offset + 0x0c) << dec  << 8 << endl;

                //            cout << "matrix 4: " <<  hex << Binary::get_uint32(gxx, gpu_offset + 0x10) << dec  << 8 << endl;
                //            cout << "matrix 5: " <<  hex << Binary::get_uint32(gxx, gpu_offset + 0x14) << dec  << 8 << endl;
                //            cout << "matrix 6: " <<  hex << Binary::get_uint32(gxx, gpu_offset + 0x18) << dec  << 8 << endl;

                //            cout << "matrix 7: " <<  hex << Binary::get_uint32(gxx, gpu_offset + 0x1C) << dec  << 8 << endl;
                //            cout << "matrix 8: " <<  hex << Binary::get_uint32(gxx, gpu_offset + 0x20) << dec  << 8 << endl;
                //            cout << "matrix 9: " <<  hex << Binary::get_uint32(gxx, gpu_offset + 0x24) << dec  << 8 << endl;

                //            cout << "matrix 10: " <<  hex << Binary::get_uint32(gxx, gpu_offset + 0x28) << dec  << 8 << endl;
                //            cout << "matrix 11: " <<  hex << Binary::get_uint32(gxx, gpu_offset + 0x2C) << dec  << 8 << endl;
                //            cout << "matrix 12: " <<  hex << Binary::get_uint32(gxx, gpu_offset + 0x30) << dec  << 8 << endl;
                //            cout << endl;*/


                //            cout << "xyz: (" << matrix.translation_x << ", " << matrix.translation_y << ", " << matrix.translation_z << ") " << endl;
                //            cout << "Scale xyz: (" << matrix.get_scale_x() << ", " << matrix.get_scale_y() << ", " << matrix.get_scale_z() << ") " << endl;
                //            cout << "    12" << endl;
                //            cout << "Rotation rad xyz: (" << matrix.get_rotation().x << ", " << matrix.get_rotation().y << ", " << matrix.get_rotation().z << ") " << endl;
                //            cout << "Rotation deg xyz: (" << rad2deg(matrix.get_rotation().x) << ", " << rad2deg(matrix.get_rotation().y) << ", " << rad2deg(matrix.get_rotation().z) << ") " << endl;
                //            break;
                //        }

                //        case GE_WORLDMATRIXDATA:
                //        {
                //            uint32_t data = buffer << 8;
                //            float f = Binary::u32_to_float(data);

                //            cout.setf(ios::fixed, ios::floatfield);
                //            cout.setf(ios::showpoint);
                //            cout << "World matrix set: " << f << endl;

                //            break;
                //        }

                //        case GE_TEXSCALEU:
                //        {
                //            uint32_t f = buffer << 8;

                //            cout.setf(ios::fixed, ios::floatfield);
                //            cout.setf(ios::showpoint);
                //            cout << "Texture U scale: " << Binary::u32_to_float(f) << endl;

                //            break;
                //        }

                //        case GE_TEXSCALEV:
                //        {
                //            uint32_t f = buffer << 8;

                //            cout.setf(ios::fixed, ios::floatfield);
                //            cout.setf(ios::showpoint);
                //            cout << "Texture V scale: " << Binary::u32_to_float(f) << endl;

                //            break;
                //        }

                //        case GE_TEXOFFSETU:
                //        {
                //            uint32_t f = buffer << 8;

                //            cout.setf(ios::fixed, ios::floatfield);
                //            cout.setf(ios::showpoint);
                //            cout << "Texture U offset: " << Binary::u32_to_float(f) << endl;

                //            break;
                //        }

                //        case GE_TEXOFFSETV:
                //        {
                //            uint32_t f = buffer << 8;

                //            cout.setf(ios::fixed, ios::floatfield);
                //            cout.setf(ios::showpoint);
                //            cout << "Texture V offset: " << Binary::u32_to_float(f) << endl;

                //            break;
                //        }

                //        case GE_MATERIALAMBIENT:
                //        {
                //            uint32_t color = buffer << 8 >> 8;
                //            cout << "Material ambient color: 0x" << hex << color << dec << endl;
                //        }
                //        case GE_MATERIALALPHA:
                //        {
                //            uint32_t color = buffer << 8 >> 8;

                //            cout.setf(ios::fixed, ios::floatfield);
                //            cout.setf(ios::showpoint);
                //            cout << "Material alpha color: 0x" << hex << color << dec << endl;

                //            break;
                //        }

                //        }


                //        buffer = Binary::get_uint32(gxx, gpu_offset);
                //    }

                //    cout << endl;

                //    for (uint32_t instr : unkownInstr)
                //    {
                //        cout << "Unknown istruction found: 0x" << hex << instr << dec << endl;
                //    }

                //    cout << endl;
                //}
            }
        }


    }


    {}
    {
        //uint32_t offset_1 = Binary::get_uint32(gxx, section_c + 0x10);
        //uint32_t offset_2 = Binary::get_uint32(gxx, offset_1);
        //uint32_t gpu_offset = Binary::get_uint32(gxx, offset_2);

        //uint32_t buffer = 0x0;
        //buffer = Binary::get_uint32(gxx, gpu_offset);

        //vector<uint32_t> unkownInstr;

        //uint32_t vertexAddr;
        //uint32_t indexAddr;
        //vector<uint32_t> returnAddr;
        //bool end = false;
        //
        //while (!end)
        //{
        //    ///We go over the most crucial GPU instructions for us
        //    gpu_offset += 0x4;

        //    switch (buffer >> 24)
        //    {
        //    default:
        //    {
        //        uint32_t data = buffer >> 24;
        //        //cout << "---UNKNOWN: " << hex << "0x" << data << dec << endl;
        //        if (!std::count(unkownInstr.begin(), unkownInstr.end(), data))
        //        {
        //            unkownInstr.push_back(data);
        //        }
        //    }

        //    case GE_NOP:
        //    {
        //        uint32_t data = buffer << 8 >> 8;
        //        if (data != 0)
        //        {
        //            cout << hex << "NOP: data= 0x" << data << dec << endl;
        //        }
        //        else
        //        {
        //            cout << "NOP" << endl;
        //        }
        //        break;
        //    }

        //    case GE_VADDR:
        //    {
        //        uint32_t data = buffer << 8 >> 8;
        //        if (data != 0)
        //        {
        //            vertexAddr = data;
        //            cout << "VADDR: " << data << endl;
        //        }
        //        break;
        //    }

        //    case GE_IADDR:
        //    {
        //        uint32_t data = buffer << 8 >> 8;
        //        if (data != 0)
        //        {
        //            indexAddr = data;
        //            cout << "IADDR: " << data << endl;
        //        }
        //        break;
        //    }

        //    case GE_PRIM:
        //    {
        //        uint32_t count = buffer & 0xFFFF;
        //        uint32_t type = (buffer >> 16) & 7;

        //        static const char* types[8] =
        //        {
        //            "POINTS",
        //            "LINES",
        //            "LINE_STRIP",
        //            "TRIANGLES",
        //            "TRIANGLE_STRIP",
        //            "TRIANGLE_FAN",
        //            "RECTANGLES",
        //            "CONTINUE_PREVIOUS",
        //        };

        //        cout << "DRAW PRIM \"" << (type < 7 ? types[type] : "INVALID") << "\" count= " << count << " vaddr= " << vertexAddr << endl;

        //        break;
        //    }

        //    case GE_JUMP:
        //    {
        //        uint32_t addr = buffer << 8 >> 8;
        //        returnAddr.push_back(gpu_offset);
        //        gpu_offset = addr;
        //        cout << "Jump to: " << hex << "0x" << addr << dec << endl;
        //        break;
        //    }
        //    case GE_CALL:
        //    {
        //        uint32_t addr = buffer << 8 >> 8;
        //        returnAddr.push_back(gpu_offset);
        //        gpu_offset = addr;
        //        cout << "Call to: " << hex << "0x" << addr << dec << endl;
        //        break;
        //    }
        //    case GE_RET:
        //    {
        //        if(returnAddr.size() > 0)
        //        {
        //            uint32_t addr = returnAddr.back();
        //            returnAddr.pop_back();
        //            gpu_offset = addr;
        //            cout << "Return to: " << hex << "0x" << addr << dec << endl;
        //        }
        //        else
        //        {
        //            end = true;
        //        }


        //        break;
        //    }

        //    case GE_BASE:
        //    {
        //        uint32_t addr = buffer << 8 >> 8;
        //        cout << "Set base addr to: " << hex << "0x" << addr << dec << endl;
        //        break;
        //    }

        //    case GE_VERTEXTYPE:
        //    {
        //        uint32_t op = buffer;
        //        cout << "SetVertexType: ";

        //        bool through = (op & GE_VTYPE_THROUGH_MASK) == GE_VTYPE_THROUGH;
        //        int tc = (op & GE_VTYPE_TC_MASK) >> GE_VTYPE_TC_SHIFT;
        //        int col = (op & GE_VTYPE_COL_MASK) >> GE_VTYPE_COL_SHIFT;
        //        int nrm = (op & GE_VTYPE_NRM_MASK) >> GE_VTYPE_NRM_SHIFT;
        //        int pos = (op & GE_VTYPE_POS_MASK) >> GE_VTYPE_POS_SHIFT;
        //        int weight = (op & GE_VTYPE_WEIGHT_MASK) >> GE_VTYPE_WEIGHT_SHIFT;
        //        int weightCount = ((op & GE_VTYPE_WEIGHTCOUNT_MASK) >> GE_VTYPE_WEIGHTCOUNT_SHIFT) + 1;
        //        int morphCount = (op & GE_VTYPE_MORPHCOUNT_MASK) >> GE_VTYPE_MORPHCOUNT_SHIFT;
        //        int idx = (op & GE_VTYPE_IDX_MASK) >> GE_VTYPE_IDX_SHIFT;

        //        static const char* colorNames[] = {
        //            NULL,
        //            "unsupported1",
        //            "unsupported2",
        //            "unsupported3",
        //            "BGR 565",
        //            "ABGR 1555",
        //            "ABGR 4444",
        //            "ABGR 8888",
        //        };
        //        static const char* typeNames[] = {
        //            NULL,
        //            "u8",
        //            "u16",
        //            "float",
        //        };
        //        static const char* typeNamesI[] = {
        //            NULL,
        //            "u8",
        //            "u16",
        //            "u32",
        //        };
        //        static const char* typeNamesS[] = {
        //            NULL,
        //            "s8",
        //            "s16",
        //            "float",
        //        };

        //        if (through)
        //            cout << "through, ";
        //        if (typeNames[tc])
        //            cout << typeNames[tc] << " texcoords, ";

        //        if (colorNames[col])
        //            cout << colorNames[col] << " colors, ";

        //        if (typeNames[nrm])
        //            cout << typeNamesS[nrm] << " normals, ";

        //        if (typeNames[pos])
        //            cout << typeNamesS[pos] << " positions, ";

        //        if (typeNames[weight])
        //            cout << typeNames[weight] << " weights " << weightCount << ", ";
        //        else if (weightCount > 1)
        //            cout << "unknown weights (%d), " << weightCount;

        //        if (morphCount > 0)
        //            cout << morphCount << " morphs, ";

        //        if (typeNamesI[idx])
        //            cout << typeNamesI[idx] << " indexes";

        //        cout << endl;

        //        break;
        //    }
        //    
        //    case GE_TEXTUREMAPENABLE:
        //    {
        //        uint32_t flag = buffer << 8 >> 8;
        //        cout << "Texture map enable: " << hex << flag << dec << endl;
        //        break;
        //    }

        //    case GE_WORLDMATRIXNUMBER:
        //    {
        //        /* PPSSPP Source
        //        if (data & ~0xF)
        //            snprintf(buffer, bufsize, "World # %i (extra %x)", data & 0xF, data);
        //        else
        //            snprintf(buffer, bufsize, "World # %i", data & 0xF);
        //        */
        //        uint32_t f = buffer << 8;

        //        cout.setf(ios::fixed, ios::floatfield);
        //        cout.setf(ios::showpoint);
        //        cout << "World: # " << (int)Binary::u32_to_float(f) << endl;

        //        break;
        //    }

        //    case GE_WORLDMATRIXDATA:
        //    {
        //        uint32_t f = buffer << 8;

        //        cout.setf(ios::fixed, ios::floatfield);
        //        cout.setf(ios::showpoint);
        //        cout << "World matrix set: " << Binary::u32_to_float(f) << endl;

        //        break;
        //    }

        //    case GE_TEXSCALEU:
        //    {
        //        uint32_t f = buffer << 8;

        //        cout.setf(ios::fixed, ios::floatfield);
        //        cout.setf(ios::showpoint);
        //        cout << "Texture U scale: " << Binary::u32_to_float(f) << endl;

        //        break;
        //    }

        //    case GE_TEXSCALEV:
        //    {
        //        uint32_t f = buffer << 8;

        //        cout.setf(ios::fixed, ios::floatfield);
        //        cout.setf(ios::showpoint);
        //        cout << "Texture V scale: " << Binary::u32_to_float(f) << endl;

        //        break;
        //    }

        //    case GE_TEXOFFSETU:
        //    {
        //        uint32_t f = buffer << 8;

        //        cout.setf(ios::fixed, ios::floatfield);
        //        cout.setf(ios::showpoint);
        //        cout << "Texture U offset: " << Binary::u32_to_float(f) << endl;

        //        break;
        //    }

        //    case GE_TEXOFFSETV:
        //    {
        //        uint32_t f = buffer << 8;

        //        cout.setf(ios::fixed, ios::floatfield);
        //        cout.setf(ios::showpoint);
        //        cout << "Texture V offset: " << Binary::u32_to_float(f) << endl;

        //        break;
        //    }

        //    case GE_MATERIALAMBIENT:
        //    {
        //        uint32_t color = buffer << 8 >> 8;
        //        cout << "Material ambient color: 0x" << hex << color << dec << endl;
        //    }
        //    case GE_MATERIALALPHA:
        //    {
        //        uint32_t color = buffer << 8 >> 8;

        //        cout.setf(ios::fixed, ios::floatfield);
        //        cout.setf(ios::showpoint);
        //        cout << "Material alpha color: 0x" << hex << color << dec << endl;

        //        break;
        //    }

        //    }


        //    buffer = Binary::get_uint32(gxx, gpu_offset);
        //}
        //
        //cout << endl;

        //for (uint32_t instr : unkownInstr)
        //{
        //    cout << "Unknown istruction found: 0x" << hex << instr << dec << endl;
        //}

        //cout << endl;
    }

    system("pause");
    return 0;
}

uint32_t bgr_to_rgba(uint16_t bgr565)
{
    /// bgr_to_rgba by Owocek
    uint8_t b = bgr565 >> 11;
    uint8_t g = uint16_t(bgr565 << 5) >> 10;
    uint8_t r = uint16_t(bgr565 << 11) >> 11;

    uint8_t red = (r * 527 + 23) >> 6;
    uint8_t green = (g * 259 + 33) >> 6;
    uint8_t blue = (b * 527 + 23) >> 6;

    return (uint32_t(red) << 24) + (uint32_t(green) << 16) + (uint32_t(blue) << 8) + 0xFF;
}

float rad2deg(float angle) {
    return angle * 180.0 / Math_PI;
}
