#include "imgui.h"
#include "raylib.h"
#include "rlImGui.h"
#include <array>
#include <functional>

using ConverterFunc = std::function<double(int, int, double)>;

constexpr double ERR = -1.0;

typedef enum ConversionType {
  LENGTH = 0,
  WEIGHT,
  TEMPERATURE,
  VOLUME,
  AREA,
  SPEED,
  TIME,
  CURRENCY,
  CONVERSION_TYPE_COUNT,
} ConversionType;

typedef enum LengthConversionUnits {
  METER = 0,
  KILOMETER,
  CENTIMETER,
  MILLIMETER,
  INCH,
  FOOT,
  YARD,
  MILE,
  LENGTH_UNITS_COUNT,
} LengthConversionUnits;

typedef enum WeightConversionUnits {
  KILOGRAM = 0,
  GRAM,
  MILLIGRAM,
  POUND,
  OUNCE,
  WEIGHT_UNITS_COUNT,
} WeightConversionUnits;

typedef enum TemperatureConversionUnits {
  CELSIUS = 0,
  FAHRENHEIT,
  KELVIN,
  TEMPERATURE_UNITS_COUNT,
} TemperatureConversionUnits;

typedef enum VolumeConversionUnits {
  LITER = 0,
  MILLILITER,
  CUBIC_METER,
  GALLON,
  PINT,
  QUART,
  CUP,
  VOLUME_UNITS_COUNT,
} VolumeConversionUnits;

typedef enum AreaConversionUnits {
  SQUARE_METER = 0,
  SQUARE_KILOMETER,
  SQUARE_FOOT,
  ACRE,
  HECTARE,
  AREA_UNITS_COUNT,
} AreaConversionUnits;

typedef enum SpeedConversionUnits {
  METER_PER_SECOND = 0,
  KILOMETER_PER_HOUR,
  MILE_PER_HOUR,
  KNOT,
  SPEED_UNITS_COUNT,
} SpeedConversionUnits;

typedef enum TimeConversionUnits {
  MILLISECOND = 0,
  SECOND,
  MINUTE,
  HOUR,
  DAY,
  WEEK,
  MONTH,
  YEAR,
  TIME_UNITS_COUNT,
} TimeConversionUnits;

typedef enum CurrencyConversionUnits {
  USD = 0,
  EUR,
  JPY,
  GBP,
  CNY,
  AUD,
  CAD,
  CHF,
  CURRENCY_UNITS_COUNT,
} CurrencyConversionUnits;

const char *ConversionTypes[] = {"LENGTH", "WEIGHT", "TEMPERATURE", "VOLUME",
                                 "AREA",   "SPEED",  "TIME",        "CURRENCY"};
const char *LengthUnits[] = {"METER", "KILOMETER", "CENTIMETER", "MILLIMETER",
                             "INCH",  "FOOT",      "YARD",       "MILE"};
const char *WeightUnits[] = {"KILOGRAM", "GRAM", "MILLIGRAM", "POUND", "OUNCE"};
const char *TemperatureUnits[] = {"CELSIUS", "FAHRENHEIT", "KELVIN"};
const char *VolumeUnits[] = {"LITER", "MILLILITER", "CUBIC_METER", "GALLON",
                             "PINT",  "QUART",      "CUP"};
const char *AreaUnits[] = {"SQUARE_METER", "SQUARE_KILOMETER", "SQUARE_FOOT",
                           "ACRE", "HECTARE"};
const char *SpeedUnits[] = {"METER_PER_SECOND", "KILOMETER_PER_HOUR",
                            "MILE_PER_HOUR", "KNOT"};
const char *TimeUnits[] = {"MILLISECOND", "SECOND", "MINUTE", "HOUR",
                           "DAY",         "WEEK",   "MONTH",  "YEAR"};
const char *CurrencyUnits[] = {"USD", "EUR", "JPY", "GBP",
                               "CNY", "AUD", "CAD", "CHF"};

const int UnitCounts[] = {LENGTH_UNITS_COUNT,      WEIGHT_UNITS_COUNT,
                          TEMPERATURE_UNITS_COUNT, VOLUME_UNITS_COUNT,
                          AREA_UNITS_COUNT,        SPEED_UNITS_COUNT,
                          TIME_UNITS_COUNT,        CURRENCY_UNITS_COUNT};

const char **UnitStrings[] = {LengthUnits, WeightUnits,  TemperatureUnits,
                              VolumeUnits, AreaUnits,    SpeedUnits,
                              TimeUnits,   CurrencyUnits};

double ConvertLength(int fromUnit, int toUnit, double value) {
  // Convert from source unit to meters
  double inMeters = 0.0;
  switch (fromUnit) {
  case METER:
    inMeters = value;
    break;
  case KILOMETER:
    inMeters = value * 1000.0;
    break;
  case CENTIMETER:
    inMeters = value / 100.0;
    break;
  case MILLIMETER:
    inMeters = value / 1000.0;
    break;
  case INCH:
    inMeters = value * 0.0254;
    break;
  case FOOT:
    inMeters = value * 0.3048;
    break;
  case YARD:
    inMeters = value * 0.9144;
    break;
  case MILE:
    inMeters = value * 1609.34;
    break;
  default:
    return ERR;
  }

  // Convert from meters to target unit
  switch (toUnit) {
  case METER:
    return inMeters;
  case KILOMETER:
    return inMeters / 1000.0;
  case CENTIMETER:
    return inMeters * 100.0;
  case MILLIMETER:
    return inMeters * 1000.0;
  case INCH:
    return inMeters / 0.0254;
  case FOOT:
    return inMeters / 0.3048;
  case YARD:
    return inMeters / 0.9144;
  case MILE:
    return inMeters / 1609.34;
  default:
    return ERR;
  }
}

double ConvertWeight(int fromUnit, int toUnit, double value) {
  // Convert to kilograms
  double inKg = 0.0;
  switch (fromUnit) {
  case KILOGRAM:
    inKg = value;
    break;
  case GRAM:
    inKg = value / 1000.0;
    break;
  case MILLIGRAM:
    inKg = value / 1e6;
    break;
  case POUND:
    inKg = value * 0.453592;
    break;
  case OUNCE:
    inKg = value * 0.0283495;
    break;
  default:
    return ERR;
  }

  // Convert from kilograms
  switch (toUnit) {
  case KILOGRAM:
    return inKg;
  case GRAM:
    return inKg * 1000.0;
  case MILLIGRAM:
    return inKg * 1e6;
  case POUND:
    return inKg / 0.453592;
  case OUNCE:
    return inKg / 0.0283495;
  default:
    return ERR;
  }
}

double ConvertTemperature(int fromUnit, int toUnit, double value) {
  double celsius = 0.0;

  // First convert to Celsius
  switch (fromUnit) {
  case CELSIUS:
    celsius = value;
    break;
  case FAHRENHEIT:
    celsius = (value - 32.0) * 5.0 / 9.0;
    break;
  case KELVIN:
    celsius = value - 273.15;
    break;
  default:
    return ERR;
  }

  // Then convert to target
  switch (toUnit) {
  case CELSIUS:
    return celsius;
  case FAHRENHEIT:
    return (celsius * 9.0 / 5.0) + 32.0;
  case KELVIN:
    return celsius + 273.15;
  default:
    return ERR;
  }
}

double ConvertVolume(int fromUnit, int toUnit, double value) {
  // Convert to liters
  double inLiters = 0.0;
  switch (fromUnit) {
  case LITER:
    inLiters = value;
    break;
  case MILLILITER:
    inLiters = value / 1000.0;
    break;
  case CUBIC_METER:
    inLiters = value * 1000.0;
    break;
  case GALLON:
    inLiters = value * 3.78541;
    break; // US gallon
  case PINT:
    inLiters = value * 0.473176;
    break; // US pint
  case QUART:
    inLiters = value * 0.946353;
    break; // US quart
  case CUP:
    inLiters = value * 0.24;
    break; // approx
  default:
    return ERR;
  }

  // Convert from liters
  switch (toUnit) {
  case LITER:
    return inLiters;
  case MILLILITER:
    return inLiters * 1000.0;
  case CUBIC_METER:
    return inLiters / 1000.0;
  case GALLON:
    return inLiters / 3.78541;
  case PINT:
    return inLiters / 0.473176;
  case QUART:
    return inLiters / 0.946353;
  case CUP:
    return inLiters / 0.24;
  default:
    return ERR;
  }
}

double ConvertArea(int fromUnit, int toUnit, double value) {
  // Convert to square meters
  double inSqMeters = 0.0;
  switch (fromUnit) {
  case SQUARE_METER:
    inSqMeters = value;
    break;
  case SQUARE_KILOMETER:
    inSqMeters = value * 1e6;
    break;
  case SQUARE_FOOT:
    inSqMeters = value * 0.092903;
    break;
  case ACRE:
    inSqMeters = value * 4046.86;
    break;
  case HECTARE:
    inSqMeters = value * 10000.0;
    break;
  default:
    return ERR;
  }

  // Convert from square meters
  switch (toUnit) {
  case SQUARE_METER:
    return inSqMeters;
  case SQUARE_KILOMETER:
    return inSqMeters / 1e6;
  case SQUARE_FOOT:
    return inSqMeters / 0.092903;
  case ACRE:
    return inSqMeters / 4046.86;
  case HECTARE:
    return inSqMeters / 10000.0;
  default:
    return ERR;
  }
}

double ConvertSpeed(int fromUnit, int toUnit, double value) {
  // Convert to meters per second
  double inMps = 0.0;
  switch (fromUnit) {
  case METER_PER_SECOND:
    inMps = value;
    break;
  case KILOMETER_PER_HOUR:
    inMps = value / 3.6;
    break;
  case MILE_PER_HOUR:
    inMps = value * 0.44704;
    break;
  case KNOT:
    inMps = value * 0.514444;
    break;
  default:
    return ERR;
  }

  // Convert from meters per second
  switch (toUnit) {
  case METER_PER_SECOND:
    return inMps;
  case KILOMETER_PER_HOUR:
    return inMps * 3.6;
  case MILE_PER_HOUR:
    return inMps / 0.44704;
  case KNOT:
    return inMps / 0.514444;
  default:
    return ERR;
  }
}

double ConvertTime(int fromUnit, int toUnit, double value) {
  // Convert to seconds
  double inSeconds = 0.0;
  switch (fromUnit) {
  case MILLISECOND:
    inSeconds = value / 1000.0;
    break;
  case SECOND:
    inSeconds = value;
    break;
  case MINUTE:
    inSeconds = value * 60.0;
    break;
  case HOUR:
    inSeconds = value * 3600.0;
    break;
  case DAY:
    inSeconds = value * 86400.0;
    break;
  case WEEK:
    inSeconds = value * 604800.0;
    break;
  case MONTH:
    inSeconds = value * 2629800.0;
    break; // ~30.44 days
  case YEAR:
    inSeconds = value * 31557600.0;
    break; // 365.25 days
  default:
    return ERR;
  }

  // Convert from seconds
  switch (toUnit) {
  case MILLISECOND:
    return inSeconds * 1000.0;
  case SECOND:
    return inSeconds;
  case MINUTE:
    return inSeconds / 60.0;
  case HOUR:
    return inSeconds / 3600.0;
  case DAY:
    return inSeconds / 86400.0;
  case WEEK:
    return inSeconds / 604800.0;
  case MONTH:
    return inSeconds / 2629800.0;
  case YEAR:
    return inSeconds / 31557600.0;
  default:
    return ERR;
  }
}

double ConvertCurrency(int fromUnit, int toUnit, double value) {
  // Example static exchange rates relative to USD
  static const double exchangeRates[CURRENCY_UNITS_COUNT] = {
      1.0,   // USD
      0.85,  // EUR
      110.0, // JPY
      0.75,  // GBP
      6.5,   // CNY
      1.35,  // AUD
      1.25,  // CAD
      0.92   // CHF
  };

  if (fromUnit < 0 || fromUnit >= CURRENCY_UNITS_COUNT || toUnit < 0 ||
      toUnit >= CURRENCY_UNITS_COUNT) {
    return ERR;
  }

  // Convert from source currency to USD
  double inUSD = value / exchangeRates[fromUnit];

  // Convert from USD to target currency
  return inUSD * exchangeRates[toUnit];
}

// see *ConversionUnits enums for unit indices
double Convert(ConversionType type, int fromUnit, int toUnit, double value) {
  static const std::array<ConverterFunc, CONVERSION_TYPE_COUNT> converters = {
      ConvertLength, ConvertWeight, ConvertTemperature, ConvertVolume,
      ConvertArea,   ConvertSpeed,  ConvertTime,        ConvertCurrency};

  if (type < 0 || type >= CONVERSION_TYPE_COUNT) {
    return ERR;
  }

  return converters[type](fromUnit, toUnit, value);
}

int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(800, 600, "Unit Converter");
  SetWindowMinSize(640, 480);
  SetTargetFPS(30);
  SetExitKey(KEY_NULL);
  InitAudioDevice();

  rlImGuiSetup(true);

  ImGui::GetStyle().FontScaleMain = 2;

  static int selectedFrom[CONVERSION_TYPE_COUNT] = {0};
  static int selectedTo[CONVERSION_TYPE_COUNT] = {0};
  static double value[CONVERSION_TYPE_COUNT] = {0.0};

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(GetColor(0x181818FF));

    rlImGuiBegin();

    if (IsWindowResized()) {
      ImGui::SetNextWindowPos(ImVec2(0, 20));
      ImGui::SetNextWindowSize(
          ImVec2(GetRenderWidth(), GetRenderHeight() - 20));
    }
    if (ImGui::Begin("##empty")) {
      if (ImGui::BeginTabBar("##empty")) {
        for (int i = 0; i < CONVERSION_TYPE_COUNT; ++i) {
          if (ImGui::BeginTabItem(ConversionTypes[i])) {
            // From combo
            ImGui::Text("From:");
            ImGui::SameLine();
            if (ImGui::BeginCombo("##from", UnitStrings[i][selectedFrom[i]])) {
              for (int j = 0; j < UnitCounts[i]; ++j) {
                bool isSelected = (selectedFrom[i] == j);
                if (ImGui::Selectable(UnitStrings[i][j], isSelected))
                  selectedFrom[i] = j;
                if (isSelected)
                  ImGui::SetItemDefaultFocus();
              }
              ImGui::EndCombo();
            }

            // To combo
            ImGui::Text("To:");
            ImGui::SameLine();
            if (ImGui::BeginCombo("##to", UnitStrings[i][selectedTo[i]])) {
              for (int j = 0; j < UnitCounts[i]; ++j) {
                bool isSelected = (selectedTo[i] == j);
                if (ImGui::Selectable(UnitStrings[i][j], isSelected))
                  selectedTo[i] = j;
                if (isSelected)
                  ImGui::SetItemDefaultFocus();
              }
              ImGui::EndCombo();
            }

            // Value input
            ImGui::InputDouble("Value", &value[i]);

            // You can call your Convert function here
            double result = Convert((ConversionType)i, selectedFrom[i],
                                    selectedTo[i], value[i]);

            ImGui::LabelText("Result", "%.3f", result);

            ImGui::EndTabItem();
          }
        }
        ImGui::EndTabBar();
      }
      ImGui::End();
    }

    rlImGuiEnd();

    DrawFPS(0, 0);
    EndDrawing();
  }

  rlImGuiShutdown();

  CloseAudioDevice();
  CloseWindow();
  return 0;
}
