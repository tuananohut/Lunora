#include "ModelLoader.h"

bool LoadModelFromFile(char filename[],
                       Vertex** outVertices,
                       UINT* outVertexCount,
                       unsigned long** outIndices,
                       UINT* outIndexCount)
{
    FILE* file = fopen(filename, "r");
    if (!file)
        return false;

    XMFLOAT3 positions[1024];
    XMFLOAT4 colors[1024];
    unsigned long indicesTemp[2048];
    int posCount = 0, colCount = 0, idxCount = 0;

    char type[2];
    while (fscanf(file, "%1s", type) == 1)
    {
        if (strcmp(type, "v") == 0)
        {
            float x, y, z;
            if (fscanf(file, "%f %f %f", &x, &y, &z) == 3)
                positions[posCount++] = XMFLOAT3(x, y, z);
        }
        else if (strcmp(type, "c") == 0)
        {
            float r, g, b, a;
            if (fscanf(file, "%f %f %f %f", &r, &g, &b, &a) == 4)
                colors[colCount++] = XMFLOAT4(r, g, b, a);
        }
        else if (strcmp(type, "i") == 0)
        {
            unsigned long a, b, c;
            if (fscanf(file, "%lu %lu %lu", &a, &b, &c) == 3)
            {
                indicesTemp[idxCount++] = a;
                indicesTemp[idxCount++] = b;
                indicesTemp[idxCount++] = c;
            }
        }
    }

    fclose(file);

    int vertexCount = (posCount < colCount) ? posCount : colCount;
    *outVertexCount = vertexCount;

    *outVertices = (Vertex*)malloc(sizeof(Vertex) * vertexCount);
    if (!*outVertices)
        return false;

    for (int i = 0; i < vertexCount; ++i)
    {
        (*outVertices)[i].position = positions[i];
        (*outVertices)[i].color = colors[i];
    }

    *outIndices = (unsigned long*)malloc(sizeof(unsigned long) * idxCount);
    if (!*outIndices)
        return false;

    for (int i = 0; i < idxCount; ++i)
        (*outIndices)[i] = indicesTemp[i];

    *outIndexCount = idxCount;

    return true;
}
