# Guia de Uso: ScoundrelCore

Este guia explica como integrar e utilizar a biblioteca `ScoundrelCore` em seu projeto.

## Arquitetura Principal

A biblioteca é estruturada em torno de um motor de estados de jogo baseado na classe `GameLogic`.

## Passos para Integração

### 1. Implementar a lógica do jogo
Você deve herdar de `GameLogic` e implementar os métodos das fases do jogo (como `initialize_game`, `start_turn`, `combat`, etc.).

```cpp
#include "GameLogic.h"

class MyGameLogic : public GameLogic {
public:
    GamePhase initialize_game(GameContext &ctx) override {
        // Inicialização customizada
        return GamePhase::START_TURN;
    }
    // Implemente as demais fases...
};
```

### 2. Implementar a interface de usuário (UI)
A biblioteca requer uma implementação da interface de usuário (`UserInterface.h`) para se comunicar com o sistema de exibição.

```cpp
#include "ui/UserInterface.h"

class MyUI : public UserInterface {
    // Implemente os métodos virtuais de exibição e entrada
};
```

### 3. Iniciar o Loop de Jogo
Use a factory estática para criar sua lógica e conecte seus componentes:

```cpp
#include "GameLogic.h"

int main() {
    // 1. Criar a lógica
    auto game = GameLogic::create(GameType::DEFAULT);
    
    // 2. Definir a UI
    MyUI my_ui;
    game->set_interface(&my_ui);
    
    // 3. Configurar contexto (necessário instanciar GameContext)
    // game->set_context(std::make_unique<GameContext>(...));

    // 4. Rodar o loop
    game->run_game_loop();
    
    return 0;
}
```

## Pontos Importantes
- **AuthManager & PersistenceManager**: Utilize os métodos estáticos `get_current_auth_manager()` e `get_persistence_manager()` na classe `GameLogic` para acessar funcionalidades de sistema conforme necessário.
- **Contexto**: O `GameContext` mantém o estado atual dos jogadores, inventário e mundo. Certifique-se de preenchê-lo corretamente antes de iniciar o `run_game_loop`.
