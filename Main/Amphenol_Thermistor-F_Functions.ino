// Manufacturer(Amphenol) constants for calculate termistor resistance of material type F
const float a[4] = { 3.3538646E-03, 2.56544090E-04, 1.9243889E-06, 1.0969244E-07 };
const float b[4] = { 3.3540154E-03, 2.5627725E-04, 2.0829210E-06, 7.3002306E-08 };
const float c[4] = { 3.3539264E-03, 2.5609446E-04, 1.9621987E-06, 4.6045930E-08 };
const float d[4] = { 3.3368620E-03, 2.4057263E-04, -2.6687093E-06, -4.0719355E-07 };


float RtoTemp(float Thermistor_R, float Nom_R) {
	float Temp;
    if (Thermistor_R / Nom_R <= 68.6 && Thermistor_R / Nom_R > 3.274) {         //  - 50 to 0 °C range
        Temp = (1 / (a[0] +
            (a[1] * log(Thermistor_R / Nom_R)) +
            (pow(a[2] * log(Thermistor_R / Nom_R), 2)) +
            (pow(a[3] * log(Thermistor_R / Nom_R), 3)))) - 273.15;
    }
    else if (Thermistor_R / Nom_R <= 3.274 && Thermistor_R / Nom_R > 0.36036) {    // 0 to 50 °C range
        Temp = (1 / (b[0] +
            (b[1] * log(Thermistor_R / Nom_R)) +
            (pow(b[2] * log(Thermistor_R / Nom_R), 2)) +
            (pow(b[3] * log(Thermistor_R / Nom_R), 3)))) - 273.15;
    }
    else if (Thermistor_R / Nom_R <= 0.36036 && Thermistor_R / Nom_R > 0.06831) {  // 50 to 100 °C range
        Temp = (1 / (c[0] +
            (c[1] * log(Thermistor_R / Nom_R)) +
            (pow(c[2] * log(Thermistor_R / Nom_R), 2)) +
            (pow(c[3] * log(Thermistor_R / Nom_R), 3)))) - 273.15;
    }
    else {                                                                 // 100 to 150 °C range
        Temp = (1 / (d[0] +
            (d[1] * log(Thermistor_R / Nom_R)) +
            (pow(d[2] * log(Thermistor_R / Nom_R), 2)) +
            (pow(d[3] * log(Thermistor_R / Nom_R), 3)))) - 273.15;
    }

	return Temp;
}