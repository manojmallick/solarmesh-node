/**
 * SolarMesh Node — native unit tests (PlatformIO Unity framework).
 * Run with: pio test -e native
 */
#include <unity.h>

// ── Logic under test (replicated; in prod share via lib/) ────────────────────

static constexpr float FULL_BATTERY_V = 4.15f;
static constexpr float MIN_BATTERY_V  = 3.3f;
static constexpr float LOW_THRESHOLD_V = 3.6f;

/** Clamp battery voltage to the safe operating range. */
float clampBattery(float v) {
    if (v > FULL_BATTERY_V) return FULL_BATTERY_V;
    if (v < MIN_BATTERY_V)  return MIN_BATTERY_V;
    return v;
}

/** Choose sleep interval based on battery level.
 *  Low battery → long sleep (120 s) to conserve energy.
 *  Good battery → short sleep (30 s) for frequent beacons.
 */
uint32_t adaptiveSleep(float batteryV) {
    return batteryV < LOW_THRESHOLD_V ? 120 : 30;
}

/** Apply one tick of charge (solar > 0.4) or drain (solar ≤ 0.4). */
float chargeTick(float batteryV, float solar) {
    batteryV += (solar > 0.4f ? 0.01f : -0.005f);
    return clampBattery(batteryV);
}

// ── Tests ────────────────────────────────────────────────────────────────────

void test_adaptive_sleep_low_battery_is_120() {
    TEST_ASSERT_EQUAL_UINT32(120, adaptiveSleep(3.5f));
}

void test_adaptive_sleep_good_battery_is_30() {
    TEST_ASSERT_EQUAL_UINT32(30, adaptiveSleep(3.9f));
}

void test_adaptive_sleep_at_threshold_is_30() {
    // 3.6 is NOT less than 3.6, so should be 30
    TEST_ASSERT_EQUAL_UINT32(30, adaptiveSleep(3.6f));
}

void test_adaptive_sleep_just_below_threshold_is_120() {
    TEST_ASSERT_EQUAL_UINT32(120, adaptiveSleep(3.59f));
}

void test_clamp_battery_max() {
    TEST_ASSERT_FLOAT_WITHIN(0.001f, FULL_BATTERY_V, clampBattery(5.0f));
}

void test_clamp_battery_min() {
    TEST_ASSERT_FLOAT_WITHIN(0.001f, MIN_BATTERY_V, clampBattery(2.0f));
}

void test_clamp_battery_in_range_unchanged() {
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 3.9f, clampBattery(3.9f));
}

void test_charge_tick_good_solar_increases_voltage() {
    float v = chargeTick(3.9f, 0.8f);  // solar > 0.4 → +0.01
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 3.91f, v);
}

void test_charge_tick_poor_solar_decreases_voltage() {
    float v = chargeTick(3.9f, 0.2f);  // solar ≤ 0.4 → -0.005
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 3.895f, v);
}

void test_charge_tick_does_not_exceed_full() {
    float v = chargeTick(FULL_BATTERY_V, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, FULL_BATTERY_V, v);
}

void test_charge_tick_does_not_drop_below_min() {
    float v = chargeTick(MIN_BATTERY_V, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, MIN_BATTERY_V, v);
}

// ── Runner ───────────────────────────────────────────────────────────────────

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_adaptive_sleep_low_battery_is_120);
    RUN_TEST(test_adaptive_sleep_good_battery_is_30);
    RUN_TEST(test_adaptive_sleep_at_threshold_is_30);
    RUN_TEST(test_adaptive_sleep_just_below_threshold_is_120);
    RUN_TEST(test_clamp_battery_max);
    RUN_TEST(test_clamp_battery_min);
    RUN_TEST(test_clamp_battery_in_range_unchanged);
    RUN_TEST(test_charge_tick_good_solar_increases_voltage);
    RUN_TEST(test_charge_tick_poor_solar_decreases_voltage);
    RUN_TEST(test_charge_tick_does_not_exceed_full);
    RUN_TEST(test_charge_tick_does_not_drop_below_min);
    return UNITY_END();
}
