#include "../unittest.h"

namespace test {
	using namespace scimitar::util;

	TEST(util, prefer) {
        enum class eVegetables {
            asparagus,
            broccoli,
            cauliflower,
            daikon,
            eggplant
        };

        std::vector<eVegetables> available_ingredients = {
            eVegetables::broccoli,
            eVegetables::eggplant,
            eVegetables::cauliflower
        };

        auto selected_ingredient = prefer(
            available_ingredients,
            eVegetables::asparagus,
            eVegetables::daikon
        );

        ASSERT_FALSE(selected_ingredient.has_value());   

        selected_ingredient = prefer(
            available_ingredients,
            eVegetables::daikon,
            eVegetables::cauliflower
        );

        ASSERT_EQ(*selected_ingredient, eVegetables::cauliflower);
	}
}