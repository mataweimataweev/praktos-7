#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;


class Organism {
protected:
    string species;
public:
    Organism(string species) : species(species) {}
    virtual void interact() = 0;
    virtual ~Organism() {}
    string getSpecies() const { return species; }
};

class Fish : public Organism {
protected:
    int size;
    int stepsToReproduce;
    int stepsWithoutFood;
public:
    Fish(string species, int size, int stepsToReproduce) : Organism(species), size(size), stepsToReproduce(stepsToReproduce), stepsWithoutFood(0) {}
    void interact() override {}
    int getSize() const { return size; }
    int getStepsToReproduce() const { return stepsToReproduce; }
    int getStepsWithoutFood() const { return stepsWithoutFood; }
    void increaseSize(int amount) { size += amount; }
    void decreaseSize(int amount) { size -= amount; }
    void resetStepsWithoutFood() { stepsWithoutFood = 0; }
    void incrementStepsWithoutFood() { stepsWithoutFood++; }
    void decrementStepsToReproduce() { stepsToReproduce--; }
};

class Algae : public Organism {
protected:
    int quantity;
public:
    Algae(string species, int quantity) : Organism(species), quantity(quantity) {}
    void interact() override {}
    int getQuantity() const { return quantity; }
    void decreaseQuantity(int amount) { quantity -= amount; }
};

class Aquarium {
private:
    vector<Fish*> fishes;
    vector<Algae*> algae;
    int steps;
    void checkInteractions() {
        for (Fish* fish : fishes) {
            for (Algae* alga : algae) {
                if (fish->getSpecies() == "Хищная" && alga->getQuantity() > 0) {
                    alga->decreaseQuantity(1);
                    fish->resetStepsWithoutFood();
                }
            }
        }

        for (Fish* fish : fishes) {
            for (Fish* otherFish : fishes) {
                if (fish != otherFish && fish->getSpecies() == "Хищная" && otherFish->getSpecies() == "Простая") {
                    fish->increaseSize(otherFish->getSize());
                    delete otherFish;
                    otherFish = nullptr;
                }
            }
        }

        for (Fish* fish : fishes) {
            if (fish->getSpecies() == "Простая" && !algae.empty()) {
                algae[0]->decreaseQuantity(1);
                fish->resetStepsWithoutFood();
            }
        }
    }
    void handleEvents() {
        for (Fish* fish : fishes) {
            if (fish->getStepsWithoutFood() > 3) {
                delete fish;
                fish = nullptr;
            }
            if (fish->getStepsToReproduce() == 0) {
                Fish* newFish = new Fish(fish->getSpecies(), fish->getSize(), rand() % 5 + 3); 
                fishes.push_back(newFish);
                fish->resetStepsWithoutFood();
                fish->decrementStepsToReproduce();
            }
            else {
                fish->decrementStepsToReproduce();
            }
            fish->incrementStepsWithoutFood();
        }
    }
public:
    Aquarium() : steps(0) {}
    void addFish(Fish* fish) {
        fishes.push_back(fish);
    }
    void addAlgae(Algae* alga) {
        algae.push_back(alga);
    }
    void simulate(int steps) {
        for (int i = 0; i < steps; ++i) {
            cout << "Шаг  " << this->steps++ << ":" << endl;
            checkInteractions();
            handleEvents();
            displayState();
            cout << endl;
        }
    }
    void displayState() {
        cout << "Рыба: " << endl;
        for (Fish* fish : fishes) {
            cout << "Вид: " << fish->getSpecies() << ", Размер: " << fish->getSize() << ", Шаги по воспроизведению: " << fish->getStepsToReproduce() << ", Шаги без еды: " << fish->getStepsWithoutFood() << endl;
        }
        cout << "Водоросли:" << std::endl;
        for (Algae* alga : algae) {
            cout << "Вид: " << alga->getSpecies() << ", Количество: " << alga->getQuantity() << endl;
        }
    }
    ~Aquarium() {
        for (Fish* fish : fishes) {
            delete fish;
        }
        for (Algae* alga : algae) {
            delete alga;
        }
    }
};

void showMenu() {
    cout << "=== Симулятор аквариума ===" << endl;
    cout << "1. Добавьте хищную рыбу" << endl;
    cout << "2. Добавьте простую рыбу" << endl;
    cout << "3. Добавьте водоросли" << endl;
    cout << "4. Имитировать" << endl;
    cout << "5. Выход " << endl;
    cout << "=========================" << endl;
    cout << "Введите свой выбор: ";
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(nullptr));

    Aquarium aquarium;

    int choice;
    do {
        showMenu();
        cin >> choice;
        switch (choice) {
        case 1: {
            Fish* predatoryFish = new Fish("Хищная", 10, rand() % 5 + 3); 
            aquarium.addFish(predatoryFish);
            break;
        }
        case 2: {
            Fish* simpleFish = new Fish("Простая", 5, rand() % 5 + 3); 
            aquarium.addFish(simpleFish);
            break;
        }
        case 3: {
            Algae* algae = new Algae("Водоросли", 20);
            aquarium.addAlgae(algae);
            break;
        }
        case 4: {
            int steps;
            cout << "Введите количество шагов моделирования: ";
            cin >> steps;
            aquarium.simulate(steps);
            break;
        }
        case 5: {
            cout << "Выход..." << endl;
            break;
        }
        default:
            cout << "Неверный выбор. Пожалуйста, попробуйте снова." << endl;
        }
    } while (choice != 5);

    return 0;
}
