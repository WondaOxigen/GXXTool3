#include <iostream>
#include "Binary.hpp"
#include "GE.hpp"

using namespace std;

bool debug = false;

void debug_log(string s)
{
    if(debug)
    cout << s << endl;
}

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
    uint32_t subsection_size = Binary::get_uint32(gxx, section_c + 0xC + 2 * 0x8);
        
    cout << endl << "Amount of bones: " << subsection_size << endl;

    ///We go through each bone offset to get it's name
    for (int a = 0; a < subsection_size; a++)
    {
        ///first, we grab the offset to specific bone entry
        uint32_t entry_offset = subsection_offset + 0x8 * a;

        uint32_t bone_integer = Binary::get_uint32(gxx, entry_offset);
        uint32_t bone_name_pointer = Binary::get_uint32(gxx, entry_offset + 0x4);

        string bone_name = Binary::get_string(gxx, bone_name_pointer);

        cout << "Bone " << a + 1 << ": " << bone_name << ", unk_1: " << bone_integer << endl;
    }


    system("pause");
    return 0;
}