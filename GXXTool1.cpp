#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
    cout << "GXXTool v1.0.0 by Owocek" << endl;

    if(argc > 1)
    {
        string file = argv[1];
        string workdir = argv[0];
        workdir = workdir.substr(0,workdir.find_last_of("\\")+1);

        cout << "Opening " << file << endl;

        ifstream gxx(file, ios::binary);

        uint32_t info_offset;

        gxx.seekg(0x10);
        gxx.read(reinterpret_cast<char*>(&info_offset), sizeof(uint32_t));

        cout << "Info section: 0x" << std::hex << info_offset << std::dec << endl;

        uint32_t block_pointer,blocks;

        gxx.seekg(info_offset+0x10);
        gxx.read(reinterpret_cast<char*>(&block_pointer), sizeof(uint32_t));

        gxx.seekg(info_offset+0x14);
        gxx.read(reinterpret_cast<char*>(&blocks), sizeof(uint32_t));

        cout << "Blocks start at: 0x" << std::hex << block_pointer << std::dec << ", " << blocks << " blocks" << endl;

        for(int i=0; i<blocks; i++)
        {
            uint32_t anim_pointer,frames,frameloop;
            float framerate;

            gxx.seekg(block_pointer+(i*0x10));
            gxx.read(reinterpret_cast<char*>(&anim_pointer), sizeof(uint32_t));

            gxx.seekg(block_pointer+(i*0x10)+0x4);
            gxx.read(reinterpret_cast<char*>(&frames), sizeof(uint32_t));

            gxx.seekg(block_pointer+(i*0x10)+0x8);
            gxx.read(reinterpret_cast<char*>(&framerate), sizeof(float));

            gxx.seekg(block_pointer+(i*0x10)+0xC);
            gxx.read(reinterpret_cast<char*>(&frameloop), sizeof(float));

            uint32_t anim_data;

            gxx.seekg(anim_pointer);
            gxx.read(reinterpret_cast<char*>(&anim_data), sizeof(uint32_t));

            cout << "Anim pointer: 0x" << std::hex << anim_pointer << " (anim data at: 0x" << anim_data << std::dec << "), Frames: " << frames << " Frame loop: " << frameloop << " Framerate: " << framerate << endl;

            uint32_t anim_data_size = 0x0;

            gxx.seekg(anim_data-0x40+0x8);
            gxx.read(reinterpret_cast<char*>(&anim_data_size), sizeof(uint32_t));

            cout << "Floats: " << endl;

            for(int f=0; f<anim_data_size/4; f++)
            {
                float value;

                gxx.seekg(anim_data+(f*4));
                gxx.read(reinterpret_cast<char*>(&value), sizeof(float));

                cout << std::fixed << value << " ";
            }

            cout << endl;
        }

        gxx.close();

        system("pause");
    }

    return 0;
}
