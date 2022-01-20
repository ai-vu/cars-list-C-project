#include <stdio.h>
#include <string.h>

#define STRING_SIZE 100

/*
 * create car info structure
 * make of the car (string), model of the car(string), price(int), co2 emissions g/km (float)
 * store data structures in binary mode file
 * print list of all car
 * user adds new cars from a text file
 * add new car info to end of the file
 */

int getValue (char line[50], char cmp[10], char fmt[10], char out[STRING_SIZE]);

struct car_record {
    char maker[STRING_SIZE];
    char model[STRING_SIZE];
    int price;
    float co2_em;
};

struct car_record cars[8] = {
        {"Mercedes-Benz", "EQC 400", 70000, 0},
        {"KIA", "Soul EV", 41000, 0},
        {"Audi", "A1", 75000, 128.1},
        {"BMW", "2 Series", 63000, 148.8},
        {"Ford", "Kuga", 58000, 169.3},
        {"Honda", "Jazz", 43000, 120},
        {"Land Rover", "Discovery", 68000, 250.5},
        {"Lexus", "IS", 83000, 228}
};

int main() {
    FILE *file;
    FILE *new;
    FILE *read;
    char file_name[] = "car_record.dat";
    char new_file[20];
    char *row[] = {"Maker", "Model", "Price", "CO2 Emission"};
    char line[50];

    // store structures into a binary file
    file = fopen(file_name, "wb");
    if (file == NULL)
    {
        printf("Unable to open file\n");
        return 1;
    }
    fwrite(cars, 1, sizeof(cars), file);

    // print the list
    printf("\nList of all cars\n");
    printf("%-20s | %-10s | %-10s | %-10s\n", row[0], row[1], row[2], row[3]);
    for (int i = 0; i < 8; i++) {
        printf("%-20s | %-10s | %10d | %10.2f\n", cars[i].maker, cars[i].model, cars[i].price,
               cars[i].co2_em);
    }
    fclose(file);

    // ask for new car info file
    printf("\nEnter new car file: ");
    scanf("%s", new_file);

    file = fopen(file_name, "ab");
    new = fopen(new_file, "r");

    // struct for new car
    struct car_record new_car;
    // counter of complete struct of a new car
    int carStructComplete = 0;

    while (!feof(new))
    {
        if (fgets(line, STRING_SIZE, new) != NULL)
        {
            char co[STRING_SIZE];
            char price[STRING_SIZE];
            // overwrite the struct with new car data
            if (getValue(line, "make", "%*14s%s", new_car.maker) )
            {
                // Trim end
                int size = strlen(new_car.maker);
                new_car.maker[size-4] = '\0';
                carStructComplete++;
            }
            else if (getValue(line, "model","%*15s%s", new_car.model))
            {
                // Trim end
                int size = strlen(new_car.model);
                new_car.model[size-4] = '\0';
                carStructComplete++;
            }
            else if (getValue(line, "price","%*12s%s", price))
            {
                sscanf(price, "%d", &new_car.price);
                carStructComplete++;
            }
            else if (getValue(line, "co2","%*10s%s", co))
            {
                sscanf(co, "%f", &new_car.co2_em);
                carStructComplete++;
            }
            // when struct of each car if filled completely --> write into file
            if (carStructComplete >= 4)
            {
                fwrite(&new_car, sizeof(struct car_record), 1, file);
                carStructComplete = 0;
            }
        }
    }

    fclose(file);

    // print list of all cars in the file (including new cars)
    read = fopen(file_name, "r");
    if (read == NULL)
    {
        printf("Unable to open file\n");
        return 1;
    } else
    {
        printf("\nUpdated List of Cars\n");
        while (fread(&cars, sizeof(struct car_record), 1, read))
        {
            printf("%-20s | %-10s | %10d | %10.2f\n", cars->maker, cars->model,
                   cars->price, cars->co2_em);
        }
    }
    fclose(read);

    return 0;
}

// a helper to get values of lines from new car file
// return true when match is found
int getValue (char line[50], char cmp[10], char fmt[10], char out[STRING_SIZE])
{
    char *match;
    match = strstr(line, cmp);
    if (match)
    {
        sscanf(line,fmt, out);
        return 1;
    }
    return 0;
}