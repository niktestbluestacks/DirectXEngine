#include "ModeListClass.hpp"

ModelListClass::ModelListClass() {
    m_ModelInfoList = nullptr;
}

ModelListClass::ModelListClass(const ModelListClass& other) {}

ModelListClass::~ModelListClass() {}

void ModelListClass::Initialize(int numModels) {
    int i;

    // Store the number of models.
    m_modelCount = numModels;

    // Create a list array of the model information.
    m_ModelInfoList = new ModelInfoType[m_modelCount];

    srand(static_cast <unsigned int> (time(nullptr)));

    // Go through all the models and randomly generate the position.
    for (i = 0; i < m_modelCount; i++) {
        // Generate a random position in front of the viewer for the mode.
        m_ModelInfoList[i].positionX = ((static_cast <float> (rand()) - static_cast <float> (rand())) / RAND_MAX) * 10.0f;
        m_ModelInfoList[i].positionY = ((static_cast <float> (rand()) - static_cast <float> (rand())) / RAND_MAX) * 10.0f;
        m_ModelInfoList[i].positionZ = (((static_cast <float> (rand()) - static_cast <float> (rand())) / RAND_MAX) * 10.0f) + 5.0f;
    }

    return;
}

void ModelListClass::Shutdown() {
    if (m_ModelInfoList) {
        delete[] m_ModelInfoList;
        m_ModelInfoList = nullptr;
    }

    return;
}

int ModelListClass::GetModelCount() {
    return m_modelCount;
}

void ModelListClass::GetData(int index, float& positionX, float& positionY, float& positionZ) {
    positionX = m_ModelInfoList[index].positionX;
    positionY = m_ModelInfoList[index].positionY;
    positionZ = m_ModelInfoList[index].positionZ;
    return;
}

