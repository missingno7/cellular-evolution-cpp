#pragma once
//
// Created by jiri on 14/11/19.
//

class Bitmap
{
public:

    Bitmap(int width,int height)
            : width_(width)
            , height_(height)
    {
        img= new unsigned char[3*width*height];
        const int pad=(4-(3*width_)%4)%4, filesize=54+(3*width_+pad)*height_; // horizontal line must be a multiple of 4 bytes long, header is 54 bytes

        for(int i=0; i<4; i++) {
            header_[ 2+i] = (unsigned char)((filesize>>(8*i))&255);
            header_[18+i] = (unsigned char)((width_   >>(8*i))&255);
            header_[22+i] = (unsigned char)((height_  >>(8*i))&255);
        }
    }

    ~Bitmap()
    {
        delete[] img;
    }


    void SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
    {
        img[3*(x+y*width_)  ] = b;
        img[3*(x+y*width_)+1] = g;
        img[3*(x+y*width_)+2] = r;
    }

    void Write(const std::string path) {
        std::ofstream file(path, std::ios::out|std::ios::binary);
        file.write(header_, 54);
        for(int i=height_-1; i>=0; i--) {
            file.write((char*)(img+3*width_*i), 3*width_);
            file.write(padding_, pad_);
        }
        file.close();
    }

private:

    int width_;
    int height_;
    unsigned char* img;

    // Internal bitmap parameters
    char header_[54] = { 'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0, 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0 };
    char padding_[3] = { 0,0,0 };
    int pad_;
};


