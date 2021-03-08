/*
int pop = GetPop();
std::string level_text;
if (keep_above && !keep_between) level_text = "> "+std::to_string(minimum);
if (!keep_above && !keep_between) level_text = "< "+std::to_string(maximum);
if (keep_between) level_text = "> "+std::to_string(minimum)+" && < "+std::to_string(maximum);

DrawStringDecal({0, 0}, level_text, olc::YELLOW, {1.5f, 1.5f});
DrawStringDecal({416, 0}, "Score: "+std::to_string(points)+"/"+std::to_string(score), olc::YELLOW, {0.5f, 0.5f});
DrawStringDecal({416, 5}, "Time Left: "+std::to_string(int(timer)), olc::YELLOW, {0.5f, 0.5f});
DrawStringDecal({416, 10}, "Population: "+std::to_string(pop), olc::YELLOW, {0.5f, 0.5f});
*/
pub fn draw_hub() {}
