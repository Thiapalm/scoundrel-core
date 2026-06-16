#include "ExtendedGameLogic.h"
#include "PlayerClassFactory.h"
#include "action.h"
#include <string>

GamePhase ExtendedGameLogic::initialize_game(GameContext &ctx)
{
    // If player class is already set (e.g. by Factory), skip selection
    if (!ctx.get_player() || ctx.get_player()->getName() == "" || ctx.get_player()->getName() == " ") {
        auto pclass_opt = ui->select_player_class();
        if (!pclass_opt) {
            return GamePhase::END_GAME;
        }
        int pclass = *pclass_opt;

        PlayerClassType chosen_class;
        switch (pclass)
        {
        case 1:
            chosen_class = PlayerClassType::HEALER;
            break;
        case 2:
            chosen_class = PlayerClassType::WARRIOR;
            break;
        case 3:
            chosen_class = PlayerClassType::WIZARD;
            break;
        case 0:
        default:
            chosen_class = PlayerClassType::PEASANT;
            break;
        }
        auto player = create_player(chosen_class);
        ctx.set_player(std::move(player));
    }
    ctx.get_player()->on_game_start(ctx);
    ctx.get_player()->on_pre_room(ctx);
    return GamePhase::PRE_TURN;
}

GamePhase ExtendedGameLogic::pre_turn(GameContext &ctx)
{
    return GamePhase::START_TURN;
}

GamePhase ExtendedGameLogic::start_turn(GameContext &ctx)
{
    ctx.get_player()->trigger_passives(ctx, PassiveTrigger::START_TURN);
    for (const auto& msg : ctx.get_and_clear_messages()) {
        ui->show_message(msg);
    }
    ui->display_game_state(ctx);

    valid_actions = ctx.get_turn_valid_actions();
    if (valid_actions.size() == 0){
        ctx.set_game_over(false);
        return GamePhase::END_GAME;
    }

    ui->display_valid_actions(valid_actions);
    return GamePhase::PRE_COMBAT;
}

GamePhase ExtendedGameLogic::pre_combat(GameContext &ctx)
{
    ctx.get_player()->trigger_passives(ctx, PassiveTrigger::BEFORE_COMBAT);
    return GamePhase::COMBAT;
}

GamePhase ExtendedGameLogic::combat(GameContext &ctx)
{
    while (true) {
        try {
            auto action_opt = ui->request_action_index(static_cast<int>(valid_actions.size()));
            if (!action_opt) {
                // True Quit (e.g. SDL_QUIT)
                ctx.set_game_over(false);
                return GamePhase::END_GAME;
            }
            
            int action_idx = *action_opt;
            if (action_idx < 0 || action_idx >= static_cast<int>(valid_actions.size())) {
                 continue; // User clicked something else or invalid index
            }
            action = static_cast<size_t>(action_idx);

            Action* current_action = valid_actions.at(action);
            if (!current_action) continue;

            auto* room = ctx.get_room();
            if (!room) continue;

            if (current_action->name() != "Skip Room" && current_action->name() != "New Room")
            {
                auto card_opt = ui->request_card_index(room->cards_in_room());
                if (!card_opt) {
                    // True Quit
                    ctx.set_game_over(false);
                    return GamePhase::END_GAME;
                }
                
                // Support cancellation (returning -1 to retry action selection)
                if (*card_opt == -1) {
                    continue; 
                }
                
                int card_idx = *card_opt;
                if (card_idx < 1 || card_idx > room->cards_in_room()) {
                    continue; // Safety
                }
                card = card_idx;
                
                if (current_action->can_execute(ctx, &room->look_card(card - 1)))
                {
                    auto selected = ctx.pop_card_from_room(card - 1);
                    current_action->execute(ctx, std::move(selected));
                    ctx.set_last_action_name(current_action->name());
                    break; // Action executed, turn ends
                }
                else {
                    ui->show_error("Cannot execute action on this card");
                    continue; // Allow user to pick another action or target
                }
            }
            else
            {
                if (current_action->can_execute(ctx, nullptr))
                {
                    current_action->execute(ctx, nullptr);
                    ctx.set_last_action_name(current_action->name());
                    break; // Action executed, turn ends
                }
                else {
                    ui->show_error("Cannot execute " + current_action->name());
                    continue; // Allow user to pick another action
                }
            }
        } catch (const std::exception& e) {
            ui->show_error("Action error: " + std::string(e.what()));
            continue; // Recover and re-prompt
        } catch (...) {
            ui->show_error("Unknown action error");
            continue;
        }
    }
    return GamePhase::POST_COMBAT;
}

GamePhase ExtendedGameLogic::post_combat(GameContext &ctx)
{
    ctx.get_player()->trigger_passives(ctx, PassiveTrigger::AFTER_COMBAT);
    return GamePhase::TERMINATE_TURN;
}

GamePhase ExtendedGameLogic::terminate_turn(GameContext &ctx)
{
    ctx.get_player()->trigger_passives(ctx, PassiveTrigger::END_TURN);
    ctx.change_turn();
    if (ctx.get_player()->current_life() <= 0) {
        ctx.set_game_over(false);
        return GamePhase::END_GAME;
    } else if (ctx.is_endgame()) {
        ctx.set_game_over(true);
        return GamePhase::END_GAME;
    }
    return GamePhase::POST_TURN;
}

GamePhase ExtendedGameLogic::post_turn(GameContext &ctx)
{
    return GamePhase::START_TURN;
}


int ExtendedGameLogic::calculate_loosing_score(GameContext &ctx)
{
    int result{0};
    // Count monsters remaining in the dungeon (non-destructively)
    auto* dungeon = ctx.get_dungeon();
    if (dungeon) {
        for (unsigned i = 0; i < dungeon->dungeon_size(); ++i) {
            const auto& card = dungeon->card_at(i);
            if (card.getType() == CardType::Monster) {
                result += static_cast<const Monster&>(card).get_damage();
            }
        }
    }
    // Count monsters remaining in the room
    auto* room = ctx.get_room();
    if (room) {
        for (int i = 0; i < room->cards_in_room(); ++i) {
            const auto& card = room->look_card(i);
            if (card.getType() == CardType::Monster) {
                result += static_cast<const Monster&>(card).get_damage();
            }
        }
    }
    return result;
}

GamePhase ExtendedGameLogic::end_game(GameContext &ctx)
{
    if (ctx.is_victory())
    {
        int score = ctx.get_player()->current_life();
        // Special rule: if player life is at maximum and last action was drink potion,
        // then final score is life + potion value
        if (ctx.get_player()->current_life() == ctx.get_player()->get_max_health() &&
            ctx.get_last_action_name() == "Drink Potion") {
            score += ctx.get_last_potion_value();
        }
        ui->show_victory(score);
    }
    else
    {
        std::string reason = (ctx.get_player()->current_life() > 0) ?
                             "No valid Actions !! (or user quit)" : "";        int score = ctx.get_player()->current_life() - calculate_loosing_score(ctx);
        ui->show_defeat(score, reason);
    }
    return GamePhase::END_GAME;
}
