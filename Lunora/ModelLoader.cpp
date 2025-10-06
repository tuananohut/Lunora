#include "ModelLoader.h"

bool LoadModelFromFile(const char* filename,
                       SimpleVertexCombined** outVertices,
                       int* outVertexCount,
                       unsigned long** outIndices,
                       int* outIndexCount)
{
    FILE* file = fopen(filename, "r");
    if (!file)
        return false;

    XMFLOAT3 positions[4096];
    XMFLOAT4 colors[4096];
    unsigned long indicesTemp[8192];

    int posCount = 0, colCount = 0, idxCount = 0;
    char type[2];

    while (fscanf(file, "%1s", type) == 1)
    {
        if (strcmp(type, "v") == 0)
        {
            float x, y, z;
            fscanf(file, "%f %f %f", &x, &y, &z);
            positions[posCount++] = XMFLOAT3(x, y, z);
        }
        else if (strcmp(type, "c") == 0)
        {
            float r, g, b, a;
            fscanf(file, "%f %f %f %f", &r, &g, &b, &a);
            colors[colCount++] = XMFLOAT4(r, g, b, a);
        }
        else if (strcmp(type, "i") == 0)
        {
            unsigned long a, b, c;
            fscanf(file, "%lu %lu %lu", &a, &b, &c);
            indicesTemp[idxCount++] = a;
            indicesTemp[idxCount++] = b;
            indicesTemp[idxCount++] = c;
        }
    }

    fclose(file);

    int vertexCount = (posCount < colCount) ? posCount : colCount;
    *outVertexCount = vertexCount;

    *outVertices = (SimpleVertexCombined*)malloc(sizeof(SimpleVertexCombined) * vertexCount);
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

    memcpy(*outIndices, indicesTemp, sizeof(unsigned long) * idxCount);
    *outIndexCount = idxCount;

    return true;
}
