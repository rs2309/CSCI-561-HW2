//
// Created by Rutvik Shah on 2/29/24.
//

#ifndef CSCI_561_HW2_GA_H
#define CSCI_561_HW2_GA_H

#include <future>
#include <memory>
#include <vector>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "RTEvaluator.h"
#include "MinMaxPlayer.h"
#include "GameEngine.h"

// Assuming Individual and necessary GA functions are defined as previously described
vector<int> timingSet2({
                               0, 10, 20, 30, 50, 60, 70, 80, 90,100, 110,  120, 130, 144});

std::vector<std::vector<int>> bestWeightsFromPreviousGenerations({
// Early Game: Emphasize mobility and strategic positioning
                                                                         {35, 10, 5, 25, 15, 120},
                                                                         {33, 12, 6, 23, 17, 120},
                                                                         {31, 14, 7, 21, 19, 120},
                                                                         {30, 16, 8, 19, 21, 120},
                                                                         {29, 18, 10, 17, 23, 120},
                                                                         {28, 20, 12, 15, 25, 120},
                                                                         {27, 22, 14, 13, 27, 120},
                                                                         {26, 24, 16, 11, 29, 120},
                                                                         {26, 26, 18, 11, 31, 120},
                                                                         {25, 28, 20, 11, 33, 120},
                                                                         {25, 30, 22, 11, 35, 120},
                                                                         {24, 32, 24, 11, 37, 120},
                                                                         {24, 34, 26, 11, 39, 120},
                                                                         {24, 36, 28, 11, 41, 120}

                                                                 });
struct Individual {
    std::vector<std::vector<int>> weights;
    double fitness = 0.0; // A measure of performance
};
Individual tournamentSelection(const std::vector<Individual>& population, int tournamentSize) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, population.size() - 1);

    Individual best = population[dis(gen)];
    for (int i = 1; i < tournamentSize; ++i) {
        const Individual& contender = population[dis(gen)];
        if (contender.fitness > best.fitness) {
            best = contender;
        }
    }
    return best;
}
Individual onePointCrossover(const Individual& parent1, const Individual& parent2) {
    Individual offspring;
    offspring.weights = parent1.weights; // Start with parent1's weights

    std::random_device rd;
    std::mt19937 gen(rd());
    // Assuming each individual has a vector<vector<int>> representing phases and their weights
    std::uniform_int_distribution<> dis(0, parent1.weights.size() - 1);

    size_t crossoverPoint = dis(gen);

    for (size_t i = crossoverPoint; i < parent2.weights.size(); ++i) {
        offspring.weights[i] = parent2.weights[i]; // Inherit from parent2 from the crossover point
    }

    return offspring;
}
void mutate(Individual& individual, double mutationRate = 0.01) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-5, 5); // Adjust weights by up to +/- 5
    std::uniform_real_distribution<> chance(0.0, 1.0);

    for (auto& phaseWeights : individual.weights) {
        for (auto& weight : phaseWeights) {
            if (chance(gen) < mutationRate) {
                weight += dis(gen); // Apply mutation
                // Ensure weight is within a valid range
                weight = std::max(0, std::min(weight, 100));
            }
        }
    }
}
std::vector<Individual> initializePopulation(size_t populationSize, size_t phaseCount, size_t weightCount) {
    std::vector<Individual> population;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 100); // Assuming weight values range from 0 to 100

    for (size_t i = 0; i < populationSize; ++i) {
        Individual ind;
        ind.weights.resize(phaseCount, std::vector<int>(weightCount));
        for (auto& phaseWeights : ind.weights) {
            std::generate(phaseWeights.begin(), phaseWeights.end(), [&]() { return dis(gen); });
        }
        population.push_back(ind);
    }

    return population;
}
double simulateGame(const Individual& individual) {
    double outcome;

//    // Create AI player with individual's weights
    std::unique_ptr<RTEvaluator> rtEvaluator = std::make_unique<RTEvaluator>(timingSet2,individual.weights);
    std::unique_ptr<Player> aiPlayer1 = std::make_unique<MinMaxPlayer>(0, std::move(rtEvaluator), 2);

    // Setup the opponent player using the best weights from previous generations
    std::unique_ptr<RTEvaluator> opponentEvaluator = std::make_unique<RTEvaluator>(timingSet2,bestWeightsFromPreviousGenerations);
    std::unique_ptr<Player> opponentPlayer1 = std::make_unique<MinMaxPlayer>(1, std::move(opponentEvaluator), 2);

    // Initialize the game engine with both players
    GameEngine game1(std::move(aiPlayer1), std::move(opponentPlayer1));
    // Run the game and determine the outcome as before
    outcome+=game1.run();

    std::unique_ptr<RTEvaluator> rtEvaluator2 = std::make_unique<RTEvaluator>(timingSet2,individual.weights);
    std::unique_ptr<Player> aiPlayer2 = std::make_unique<MinMaxPlayer>(1, std::move(rtEvaluator2), 1);

    std::unique_ptr<RTEvaluator> opponentEvaluator2 = std::make_unique<RTEvaluator>(timingSet2,bestWeightsFromPreviousGenerations);
    std::unique_ptr<Player> opponentPlayer2 = std::make_unique<MinMaxPlayer>(0, std::move(opponentEvaluator2), 1);
//
//    // Initialize the game engine with both players
    GameEngine game2(std::move(opponentPlayer2),std::move(aiPlayer2));
    // Run the game and determine the outcome as before
    outcome-=game2.run();
    cout<<"outcome"<<outcome<<endl;
    return outcome;
}
void saveWeightsToFile(const std::vector<std::vector<int>>& weights, int generation) {
    // Construct a filename based on the generation number
    std::string filename = "bestWeights_Generation_" + std::to_string(generation) + ".txt";

    // Open a file stream for writing
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    // Write the weights to the file
    for (const auto& weightSet : weights) {
        for (size_t i = 0; i < weightSet.size(); ++i) {
            file << weightSet[i];
            if (i < weightSet.size() - 1) file << ", "; // Separate weights with commas
        }
        file << "\n"; // New line for each phase's weights
    }

    file.close();
    std::cout << "Saved best weights to " << filename << std::endl;
}
void postGenerationUpdate(const std::vector<Individual>& population,int generation) {
    // Identify the best-performing individual
    const Individual& bestIndividual = *std::max_element(population.begin(), population.end(),
                                                         [](const Individual& a, const Individual& b) {
                                                             return a.fitness < b.fitness;
                                                         });
    // Update the default opponent's weights for the next generation
    if(bestIndividual.fitness>0){
        bestWeightsFromPreviousGenerations = bestIndividual.weights;
        saveWeightsToFile(bestIndividual.weights, generation);
    }
}
void evaluateFitness(Individual& individual) {
    // Simulate games and calculate fitness based on the outcomes
    const int numberOfGames = 2; // Number of games to simulate for each individual

    individual.fitness += simulateGame(individual);

    individual.fitness /= numberOfGames;
}

void runGA(size_t populationSize, int maxGenerations, size_t phaseCount, size_t weightCount) {
    auto population = initializePopulation(populationSize, phaseCount, weightCount);


    for (int generation = 0; generation < maxGenerations; ++generation) {
        std::cout << "Generation: " << generation << std::endl;

        // Vector to hold future objects returned by std::async
        std::vector<std::future<void>> futures;

        // Launch fitness evaluation for each individual in parallel
        futures.reserve(population.size());
        for (auto& individual : population) {
            futures.push_back(async(launch::async, evaluateFitness, ref(individual)));
        }

        // Wait for all futures to complete
        for (auto& f : futures) {
            f.get();
        }

        // Sort population by fitness
        std::sort(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
            return a.fitness > b.fitness; // Descending order
        });
        postGenerationUpdate(population,generation);

        // Create a new population through crossover and mutation
        std::vector<Individual> newPopulation;
        while (newPopulation.size() < population.size()) {
            Individual parent1 = tournamentSelection(population, 3);
            Individual parent2 = tournamentSelection(population, 3);

            Individual child = onePointCrossover(parent1, parent2);
            mutate(child);

            newPopulation.push_back(child);
        }

        // Optionally, keep the best individuals from the previous generation (elitism)
        newPopulation[0] = population[0]; // Example of keeping the best individual

        population = std::move(newPopulation);
    }

    // After GA completion, the best individual should be at the front if sorted by fitness
    std::cout << "Best Individual's Fitness: " << population.front().fitness << std::endl;
    // Optionally, output the best performing weight set
}


class GA {
public:
    static int GArun() {
        const size_t populationSize = 3;
        const int maxGenerations = 15;
        const size_t phaseCount = 14; // Corresponds to the number of phases
        const size_t weightCount = 6; // Corresponds to the number of weights per phase

        runGA(populationSize, maxGenerations, phaseCount, weightCount);

        return 0;
    }
};


#endif //CSCI_561_HW2_GA_H
