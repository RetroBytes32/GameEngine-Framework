#include <GameEngineFramework/ActorAI/neurons/WeightedLayer.h>


WeightedLayer::WeightedLayer() {
    
    for (int i=0; i < NEURAL_LAYER_WIDTH; i++) node[i]   = 0;
    for (int i=0; i < NEURAL_LAYER_WIDTH; i++) weight[i] = 1;
    
    return;
}
