#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

const int MAX_SPECIES = 10;
const int MAX_REACTIONS = 10;
const double R = 8.314; 


struct Reaction {
    double A_forward;   // Pre-exponential factor
    double Ea_forward;  // Activation energy
    double A_reverse;
    double Ea_reverse;
    int stoich[MAX_SPECIES]; // Net stoichiometry for each species
};


struct Feed {
    int species_index;
    double rate;  // mol/L/s
    double start_time;
    double stop_time;
};


void calc_rate_constants(double T, Reaction& r, double& kf, double& kr) {
    kf = r.A_forward * exp(-r.Ea_forward / (R * T));
    kr = r.A_reverse * exp(-r.Ea_reverse / (R * T));
}


void rate_of_change(int n_species, int n_reactions, Reaction reactions[], double conc[], double T, double dcdt[]) {
    for (int i = 0; i < n_species; ++i) dcdt[i] = 0;

    for (int r = 0; r < n_reactions; ++r) {
        double kf, kr;
        calc_rate_constants(T, reactions[r], kf, kr);

        double rate_f = kf, rate_r = kr;

        for (int s = 0; s < n_species; ++s) {
            int sto = reactions[r].stoich[s];
            if (sto < 0) rate_f *= pow(conc[s], -sto);
            if (sto > 0) rate_r *= pow(conc[s], sto);
        }

        for (int s = 0; s < n_species; ++s) {
            dcdt[s] += reactions[r].stoich[s] * (rate_f - rate_r);
        }
    }
}

void euler_step_open(double conc[], int n_species, int n_reactions, Reaction reactions[],
                     Feed feeds[], int n_feeds, double T, double t, double dt) {
    double dcdt[MAX_SPECIES];
    rate_of_change(n_species, n_reactions, reactions, conc, T, dcdt);

    for (int i = 0; i < n_feeds; ++i) {
        if (t >= feeds[i].start_time && t <= feeds[i].stop_time) {
            dcdt[feeds[i].species_index] += feeds[i].rate;
        }
    }

    for (int i = 0; i < n_species; ++i) {
        conc[i] += dcdt[i] * dt;
        if (conc[i] < 0) conc[i] = 0;
    }
}

int main() {
    int n_species, n_reactions, n_feeds;
    double T, dt, total_time;
    string species_names[MAX_SPECIES];
    double conc[MAX_SPECIES];

    cout << "=== ADVANCED CHEMICAL KINETICS SIMULATOR ===\n";
    cout << "Enter number of species (max 10): ";
    cin >> n_species;
    for (int i = 0; i < n_species; ++i) {
        cout << "Name of species " << i << ": ";
        cin >> species_names[i];
        cout << "Initial concentration (mol/L): ";
        cin >> conc[i];
    }

    cout << "\nEnter number of reactions (max 10): ";
    cin >> n_reactions;
    Reaction reactions[MAX_REACTIONS];

    for (int r = 0; r < n_reactions; ++r) {
        cout << "\n--- Reaction " << r + 1 << " ---\n";
        cout << "Pre-exponential factor (forward) A (L/mol/s): ";
        cin >> reactions[r].A_forward;
        cout << "Activation energy (forward) Ea (J/mol): ";
        cin >> reactions[r].Ea_forward;

        cout << "Pre-exponential factor (reverse) A (L/mol/s): ";
        cin >> reactions[r].A_reverse;
        cout << "Activation energy (reverse) Ea (J/mol): ";
        cin >> reactions[r].Ea_reverse;

        for (int s = 0; s < n_species; ++s) {
            cout << "Stoichiometry for " << species_names[s] << " (neg=reactant, pos=product): ";
            cin >> reactions[r].stoich[s];
        }
    }

    cout << "\nEnter number of open-system feeds (e.g. add/remove reactants): ";
    cin >> n_feeds;
    Feed feeds[10];
    for (int i = 0; i < n_feeds; ++i) {
        cout << "--- Feed " << i + 1 << " ---\n";
        cout << "Species index (0-based): "; cin >> feeds[i].species_index;
        cout << "Rate of feed (+ = in, - = out) [mol/L/s]: "; cin >> feeds[i].rate;
        cout << "Start time (s): "; cin >> feeds[i].start_time;
        cout << "Stop time (s): "; cin >> feeds[i].stop_time;
    }

    cout << "\nTemperature (Kelvin): "; cin >> T;
    cout << "Time step (s): "; cin >> dt;
    cout << "Total simulation time (s): "; cin >> total_time;

    cout << "\n=== SIMULATION STARTED ===\n";
    cout << fixed << setprecision(5);
    cout << "\nTime(s)";
    for (int i = 0; i < n_species; ++i) cout << "\t[" << species_names[i] << "]";
    cout << endl;

    int steps = total_time / dt;
    for (int i = 0; i <= steps; ++i) {
        double t = i * dt;
        cout << setw(7) << t;
        for (int s = 0; s < n_species; ++s) cout << "\t" << conc[s];
        cout << endl;
        euler_step_open(conc, n_species, n_reactions, reactions, feeds, n_feeds, T, t, dt);
    }

    cout << "\n=== SIMULATION COMPLETE ===\n";

    return 0;
}
