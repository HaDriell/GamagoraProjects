#include <fann.h>
#include <floatfann.h>
#include <iostream>
#include <fstream>
#include <string>


const char FANN_NETWORK_FILE[] = "lang.net";
const unsigned long FANN_LAYERS = 2;
const unsigned long FANN_INPUTS = 26;
const unsigned long FANN_OUTPUTS = 1;

//Outputs
const fann_type ENGLISH     = -1;
const fann_type FRENCH      = 0;
const fann_type PORTUGUESE  = 1;

fann_type* process_file(const std::string& path);

void train_with_text_file(fann* network, const std::string& path, fann_type desired_output);
bool test_with_text_file(fann* network, const std::string& path, fann_type desired_output);
void run_with_text_file(fann* network, const std::string& path);

int main(int argc, char** argv)
{
    //Load existing
    fann* network = fann_create_standard(FANN_LAYERS, FANN_INPUTS, FANN_OUTPUTS);
    fann_set_activation_function_output(network, FANN_SIGMOID_SYMMETRIC);
    //Create new
    if (!network)
    {
        std::cout << "Failed to load FANN. Creating a new neural network from memory" << std::endl;
        network = fann_create_standard(FANN_LAYERS, FANN_INPUTS, FANN_OUTPUTS);
        std::cout << "saving neural network for later usage." << std::endl;
    }

    unsigned long epoch = 0;
    bool ok = false;
    while (!ok)
    {
        epoch++;
        
        //Train Phase
        for (int i = 1; i <= 5; i++) train_with_text_file(network, "train/english/" + std::to_string(i) + ".txt", ENGLISH);
        for (int i = 1; i <= 5; i++) train_with_text_file(network, "train/french/" + std::to_string(i) + ".txt", FRENCH);
        for (int i = 1; i <= 5; i++) train_with_text_file(network, "train/portuguese/" + std::to_string(i) + ".txt", PORTUGUESE);

        //Test Phase 1
        if (!test_with_text_file(network, "test/english/1.txt", ENGLISH)) continue;
        if (!test_with_text_file(network, "test/french/1.txt", FRENCH)) continue;
        if (!test_with_text_file(network, "test/portuguese/1.txt", PORTUGUESE)) continue;

        //Test Phase 2 (to lower bias ?)
        if (!test_with_text_file(network, "test/english/2.txt", ENGLISH)) continue;
        if (!test_with_text_file(network, "test/french/2.txt", FRENCH)) continue;
        if (!test_with_text_file(network, "test/portuguese/2.txt", PORTUGUESE)) continue;

        ok = true;
    }

    std::cout << "Validation Epoch : " << epoch << std::endl;

    run_with_text_file(network, "test/english/1.txt");
    run_with_text_file(network, "test/french/1.txt");
    run_with_text_file(network, "test/portuguese/1.txt");
    fann_save(network, FANN_NETWORK_FILE);



    std::cout << "Program terminated. Press Enter to Exit" << std::endl;
    std::cin.get();
}


fann_type* process_file(const std::string& path)
{
    std::ifstream stream(path);


    if (!stream.is_open())
    {
        std::cout << "Failed to find text File '" << path << "'" << std::endl;
        return nullptr;
    }

    fann_type* input = new fann_type[FANN_INPUTS];
    for (int i = 0; i < FANN_INPUTS; i++)
        input[i] = 0;

    //Count Characters
    char c;
    float length = 0;
    while (stream >> c)
    {
        //Lower characters only
        c = std::tolower(c);

        if (std::isalpha(c))
        {
            input[c - 'a'] += 1;
            length += 1;
        }
    }

    //Normalize
    for (int i = 'a'; i < 'z'; i++)
    {
        input[i - 'a'] /= length;
    }

    return input;
}

void train_with_text_file(fann* network, const std::string& path, fann_type desired_output)
{
    fann_type* input = process_file(path);
    fann_train(network, input, &desired_output);
    delete[] input;
}

void run_with_text_file(fann* network, const std::string& path)
{
    fann_type* input = process_file(path);
    fann_type* output = fann_run(network, input);
    delete[] input;

    std::cout << "FANN Output (" << output[0] << ") : ";
    if (output[0] < -0.5)                       { std::cout << "English"; }
    if (output[0] >= -0.5 && output[0] < 0.5)   { std::cout << "French"; }
    if (output[0] > 0.5)                        { std::cout << "Portuguese"; }
    std::cout << std::endl;
}


bool test_with_text_file(fann* network, const std::string& path, fann_type desired_output)
{
    fann_type* input = process_file(path);
    fann_type* output = fann_run(network, input);
    delete[] input;

    if (output[0] < -0.5)                       return desired_output == ENGLISH;
    if (output[0] >= -0.5 && output[0] < 0.5)   return desired_output == FRENCH;
    if (output[0] > 0.5)                        return desired_output == PORTUGUESE;
    return false;
}