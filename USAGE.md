# Guia de Uso: ScoundrelCore

Bem-vindo ao **ScoundrelCore**! Esta biblioteca é o "cérebro" por trás de um jogo de cartas estilo *dungeon crawler* (exploração de masmorra). 

Se você é um desenvolvedor querendo criar um jogo ou apenas alguém curioso sobre como isso funciona, este guia vai te ajudar.

---

## 🎮 O que é o Scoundrel?

O Scoundrel é um jogo de sobrevivência e estratégia jogado com um baralho comum de 52 cartas. O seu objetivo é atravessar uma masmorra cheia de perigos e sair vivo!

### Como funciona o jogo:
1.  **A Masmorra:** O baralho é a sua masmorra. Ele contém monstros, armas e poções.
2.  **As Salas:** Você entra em salas com **4 cartas** por vez.
3.  **Suas Escolhas:** Em cada sala, você deve lidar com as cartas (lutar contra monstros, equipar armas ou beber poções). Você pode pular uma sala se achar muito perigosa, mas não pode pular duas vezes seguidas!
4.  **Vida (PV):** Você começa com 20 Pontos de Vida. Se chegar a 0, o jogo acaba.

### As Cartas:
*   **👹 Monstros (Paus e Espadas):** Causam dano. Você pode lutar com as mãos (perde vida) ou usar uma arma.
*   **⚔️ Armas (Ouros):** Ajudam a derrotar monstros sem perder tanta vida. Mas atenção: as armas têm "memória" e só podem derrotar monstros cada vez mais fracos.
*   **🧪 Poções (Copas):** Recuperam sua vida (até o máximo de 20).

---

## 🛠️ Para Desenvolvedores: Como usar esta Biblioteca

O **ScoundrelCore** fornece toda a lógica matemática e as regras do jogo. Você só precisa construir a "roupa" (a interface visual) para ele.

### 1. Os Três Pilares
Para fazer o jogo rodar, você interage com três partes principais:
*   **GameLogic:** É o motor do jogo. Ele decide o que acontece quando você ataca um monstro ou entra numa sala.
*   **GameContext:** É a "memória" do jogo. Ele guarda quanta vida o jogador tem, qual arma está equipada e quais cartas estão na sala.
*   **UserInterface:** É como o jogo fala com você. Você deve criar uma classe que mostre as mensagens na tela e receba os comandos do jogador.

### 2. Exemplo Simples de Integração

Aqui está um roteiro de como começar no seu código C++:

```cpp
#include "GameLogic.h"
#include "ui/UserInterface.h"

// 1. Crie sua Interface (como o jogador vê o jogo)
class MinhaInterface : public UserInterface {
    void display_room(const GameContext& ctx) override {
        // Código para mostrar as cartas na tela
    }
    // ... outros métodos de exibição
};

int main() {
    // 2. Inicialize o Motor do Jogo
    auto game = GameLogic::create(GameType::DEFAULT);
    
    // 3. Conecte sua Interface
    MinhaInterface ui;
    game->set_interface(&ui);
    
    // 4. Comece a aventura!
    game->run_game_loop();
    
    return 0;
}
```

---

## 🌟 Recursos Avançados
*   **Classes de Personagem:** Você pode jogar como **Guerreiro**, **Curandeiro** ou **Camponês**, cada um com habilidades especiais.
*   **Sistema de Persistência:** Salve recordes e configurações automaticamente.
*   **Internacionalização (i18n):** Suporte nativo para múltiplos idiomas (Português, Inglês, Espanhol).

---

## 📜 Licença e Créditos
O ScoundrelCore é baseado nas regras originais do jogo Scoundrel de Zach Gage e Kurt Bieg.
