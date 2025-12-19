#ifndef TEXTURE_H
#define TEXTURE_H

#include <d3d11.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

/*
struct TargaHeader
{
  unsigned char data1[12];
  unsigned short width;
  unsigned short height;
  unsigned char bpp;
  unsigned char data2; 
};
*/

#pragma pack(push, 1)
struct TargaHeader
{
    unsigned char  idLength;        // 0
    unsigned char  colorMapType;    // 1
    unsigned char  imageType;       // 2

    unsigned short colorMapFirst;   // 3-4
    unsigned short colorMapLength;  // 5-6
    unsigned char  colorMapEntry;   // 7

    unsigned short xOrigin;         // 8-9
    unsigned short yOrigin;         // 10-11
    unsigned short width;           // 12-13
    unsigned short height;          // 14-15
    unsigned char  bpp;             // 16
    unsigned char  imageDescriptor; // 17
};
#pragma pack(pop)

struct Texture
{
  unsigned char* m_targaData;
  ID3D11Texture2D* m_texture;
  ID3D11ShaderResourceView* m_textureView;
  int m_width, m_height; 
};

bool InitializeTexture(ID3D11Device*, ID3D11DeviceContext*, Texture*, const char*);
void ReleaseTexture(Texture*);

bool LoadTarga32Bit(Texture*, const char*); 
bool LoadTarga24Bit(Texture* texture, const char* filename);

#endif
