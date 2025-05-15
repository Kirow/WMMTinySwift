#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

#define READ_LINE_LEN	256
#define BYTES_LEN		10

uint8_t convert_double_to_varint(double coeff, char *bytes, uint8_t length);
void test(void);

int main(int argc, char *argv[])
{
	char line[READ_LINE_LEN];
	char bytes[BYTES_LEN];
	double coeff;
	uint8_t bytes_count;
	uint8_t i;
	bool first = true;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <input.COF>\n", argv[0]);
		return EXIT_FAILURE;
	}
	const char *input_path = argv[1];
	/* derive output path: same directory, base name without extension + "_cof.c" */
	/* determine directory and base name */
	char dirbuf[PATH_MAX];
	const char *basename = input_path;
	const char *slash = strrchr(input_path, '/');
	if (slash)
	{
		size_t dirlen = slash - input_path;
		strncpy(dirbuf, input_path, dirlen);
		dirbuf[dirlen] = '\0';
		basename = slash + 1;
	}
	else
	{
		strncpy(dirbuf, ".", sizeof(dirbuf));
		dirbuf[1] = '\0';
	}
	/* strip extension */
	char basebuf[PATH_MAX];
	const char *dot = strrchr(basename, '.');
	size_t baselen = dot ? (size_t)(dot - basename) : strlen(basename);
	strncpy(basebuf, basename, baselen);
	basebuf[baselen] = '\0';
	/* build output path */
	char output_path[PATH_MAX];
	snprintf(output_path, sizeof(output_path), "%s/%s_COF.c", dirbuf, basebuf);

	FILE *in = fopen(input_path, "r");
	if (in == NULL)
	{
		printf("Cannot open coefficients input file WMM.cof\n");
		return EXIT_FAILURE;
	}

	FILE *out = fopen(output_path, "w");
	if (out == NULL)
	{
		fprintf(stderr, "Cannot open output file %s\n", output_path);
		return EXIT_FAILURE;
	}

	fgets(line, READ_LINE_LEN, in);
	if (strcmp(strtok(line, " "), "2025.0") != 0)
	{
		printf("Not a 2025 coefficients file\n");
		return EXIT_FAILURE;
	}

	fprintf(out, "#include <stdint.h>\n\n");
	fprintf(out, "const uint8_t wmm_cof_entries_encoded[] =\n{");

	while (strncmp(fgets(line, READ_LINE_LEN, in), "9999", 4) != 0)
	{
		// ignore coeff numbers
		strtok(line, " ");
		strtok(NULL, " ");

		// first coeff
		coeff = atof(strtok(NULL, " "));
		bytes_count = convert_double_to_varint(coeff, bytes, BYTES_LEN);

		for (i = 0U; i < bytes_count; i++)
		{
			if (first)
			{
				first = false;
			}
			else
			{
				fprintf(out, ", ");
			}
			fprintf(out, "0x%02hhX", bytes[i]);
		}

		// second coeff
		coeff = atof(strtok(NULL, " "));
		bytes_count = convert_double_to_varint(coeff, bytes, BYTES_LEN);

		for (i = 0U; i < bytes_count; i++)
		{
			fprintf(out, ", 0x%02hhX", bytes[i]);
		}

		// third coeff
		coeff = atof(strtok(NULL, " "));
		bytes_count = convert_double_to_varint(coeff, bytes, BYTES_LEN);

		for (i = 0U; i < bytes_count; i++)
		{
			fprintf(out, ", 0x%02hhX", bytes[i]);
		}

		// fourth coeff
		coeff = atof(strtok(NULL, " \n"));
		bytes_count = convert_double_to_varint(coeff, bytes, BYTES_LEN);

		for (i = 0U; i < bytes_count; i++)
		{
			fprintf(out, ", 0x%02hhX", bytes[i]);
		}
	}

	fprintf(out, "};\n");

	fclose(out);
	fclose(in);

	printf("Conversion complete: %s\n", output_path);

	return EXIT_SUCCESS;
}

uint8_t convert_double_to_varint(double coeff, char *bytes, uint8_t length)
{
	int32_t int_coeff = (int32_t)(coeff * 10.0);
	uint8_t bytes_count = 0U;
	bool first_byte = true;
	memset(bytes, '\0', BYTES_LEN);

	do
	{
		if (first_byte)
		{
			// set sign flag
			if (int_coeff < 0)
			{
				bytes[bytes_count] = 0x40;
				int_coeff = -int_coeff;
			}

			// set data
			bytes[bytes_count] |= int_coeff & 0x3f;

			if (int_coeff > 0x3f)
			{
				bytes[bytes_count] |= 0x80;
				int_coeff = int_coeff >> 6;
				bytes_count++;
			}
			else
			{
				break;
			}

			first_byte = false;
		}
		else
		{
			// set data
			bytes[bytes_count] = int_coeff & 0x7f;

			if (int_coeff > 0x7f)
			{
				bytes[bytes_count] |= 0x80;
				int_coeff = int_coeff >> 7;
				bytes_count++;
			}
			else
			{
				break;
			}
		}

	} while (true);

	return bytes_count + 1;
}
