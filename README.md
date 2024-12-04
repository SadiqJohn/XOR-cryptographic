
# XOR Encryption Utility

This utility is a multi-threaded XOR encryptor/decryptor written in C++. It performs a simple XOR cryptographic transform on input data using a key provided from an external file. The utility supports multiple threads for parallel processing and ensures consistent output regardless of thread count.

## Features
- XOR encryption and decryption with a rotating key.
- Multi-threaded processing for large data.
- Consistent output across single-threaded and multi-threaded runs.

## Usage

### Command Syntax
```
./encryptUtil -n <number_of_threads> -k <keyfile> < input_plaintext > output_cyphertext
```

### Parameters
- `-n <number_of_threads>`: Number of threads for parallel processing.
- `-k <keyfile>`: Path to the file containing the XOR key.
- `< input_plaintext>`: Input plaintext data to be encrypted or decrypted.
- `> output_cyphertext`: Output file for the result.

### Examples

#### Encrypt with a single thread:
```bash
./encryptUtil -n 1 -k keyfile < plaintext > cyphertext
```

#### Encrypt with multiple threads (e.g., 4 threads):
```bash
./encryptUtil -n 4 -k keyfile < plaintext > cyphertext
```

#### Decrypt (using the same key):
```bash
./encryptUtil -n 4 -k keyfile < cyphertext > decrypted_plaintext
```

#### Verify the decryption:
```bash
diff plaintext decrypted_plaintext
```

### Sample Data Creation
To create sample `plaintext` and `keyfile`:
```bash
echo -n -e '' > plaintext
echo -n -e 'ðð' > keyfile
```

### File Inspection
To inspect the binary contents of files:
```bash
hexdump -C <filename>
```

## Build Instructions

### Requirements
- GCC with C++17 support
- POSIX-compliant system (Linux, macOS, etc.)

### Compilation
To compile the utility:
```bash
make
```

### Cleaning Up
To clean up the compiled files:
```bash
make clean
```

## Performance Testing
Test the utility with large files and multiple threads:
```bash
dd if=/dev/urandom bs=1M count=100 of=large_plaintext
time ./encryptUtil -n 1 -k keyfile < large_plaintext > large_cyphertext_single
time ./encryptUtil -n 4 -k keyfile < large_plaintext > large_cyphertext_multi
```

## Notes
- Ensure the `keyfile` is non-empty and matches the expected binary format.
- The program supports plaintext files of any size, even exceeding available memory.

## License
This project is licensed under the MIT License. See the LICENSE file for details.
