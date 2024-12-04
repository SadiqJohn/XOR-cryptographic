#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <cstring>

// Function to rotate the key left by one bit
void rotateKeyLeft(std::vector<uint8_t>& key) {
    uint8_t carry = 0;
    for (size_t i = 0; i < key.size(); i++) {
        uint8_t next_carry = (key[i] & 0x80) >> 7; // Capture MSB
        key[i] = (key[i] << 1) | carry;            // Left shift and add previous carry
        carry = next_carry;
    }
    if (!key.empty()) {
        key[0] |= carry; // Add the carry to the start of the key
    }
}

// Rotate the key for the thread based on its starting position
void rotateKeyForThread(std::vector<uint8_t>& key, size_t rotations) {
    for (size_t i = 0; i < rotations; i++) {
        rotateKeyLeft(key);
    }
}

// Function to process a block of data
void processBlock(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data, size_t start, size_t end, size_t key_rotation, std::vector<uint8_t>& output) {
    std::vector<uint8_t> rotated_key = key;

    // Rotate key for this thread's starting position
    rotateKeyForThread(rotated_key, key_rotation);

    size_t key_size = rotated_key.size();
    for (size_t i = start; i < end; i++) {
        output[i] = data[i] ^ rotated_key[i % key_size];
    }
}

// Entry point of the program
int main(int argc, char* argv[]) {
    int num_threads = 1;
    std::string key_file;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            num_threads = std::stoi(argv[++i]);
        } else if (strcmp(argv[i], "-k") == 0 && i + 1 < argc) {
            key_file = argv[++i];
        } else {
            std::cerr << "Usage: " << argv[0] << " -n <threads> -k <keyfile>\n";
            return 1;
        }
    }

    if (key_file.empty()) {
        std::cerr << "Key file must be provided.\n";
        return 1;
    }

    // Read the key from the file
    std::ifstream key_in(key_file, std::ios::binary);
    if (!key_in) {
        std::cerr << "Failed to open key file.\n";
        return 1;
    }
    std::vector<uint8_t> key((std::istreambuf_iterator<char>(key_in)), std::istreambuf_iterator<char>());
    key_in.close();
    if (key.empty()) {
        std::cerr << "Key file is empty.\n";
        return 1;
    }

    // Read the plaintext from stdin
    std::vector<uint8_t> plaintext((std::istreambuf_iterator<char>(std::cin)), std::istreambuf_iterator<char>());

    // Prepare output vector
    std::vector<uint8_t> output(plaintext.size());

    // Determine block size for each thread
    size_t block_size = plaintext.size() / num_threads;
    std::vector<std::thread> threads;

    // Encrypt data in parallel
    for (int t = 0; t < num_threads; t++) {
        size_t start = t * block_size;
        size_t end = (t == num_threads - 1) ? plaintext.size() : start + block_size;

        // Calculate rotations needed for this thread
        size_t key_rotation = (start % key.size()) * 8;

        threads.emplace_back(processBlock, std::cref(key), std::cref(plaintext), start, end, key_rotation, std::ref(output));
    }

    // Join threads
    for (auto& th : threads) {
        th.join();
    }

    // Write ciphertext to stdout
    std::cout.write(reinterpret_cast<const char*>(output.data()), output.size());

    return 0;
}

