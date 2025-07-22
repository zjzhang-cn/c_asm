#include <immintrin.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define WIDTH (1920<<1)
#define HEIGHT (1200<<1)
#define BLOCK_WIDTH (1920>>4)
#define BLOCK_HEIGHT (HEIGHT >> 3)
#define CHANNELS 3

int compare_rgb_blocks_avx2(const uint8_t *img1, const uint8_t *img2,
							 int width, int height, int *block_flags)
{
	int blocks_x = width / BLOCK_WIDTH;
	int blocks_y = height / BLOCK_HEIGHT;
	int block_id = 0;
	int ret=0;
	for (int by = 0; by < blocks_y; ++by)
	{
		for (int bx = 0; bx < blocks_x; ++bx)
		{
			uint32_t total_diff = 0;

			for (int y = 0; y < BLOCK_HEIGHT; ++y)
			{
				int base = ((by * BLOCK_HEIGHT + y) * width + bx * BLOCK_WIDTH) * CHANNELS;

				for (int x = 0; x < BLOCK_WIDTH * CHANNELS; x += 32)
				{
					__m256i a = _mm256_loadu_si256((__m256i *)&img1[base + x]);
					__m256i b = _mm256_loadu_si256((__m256i *)&img2[base + x]);
					__m256i diff = _mm256_xor_si256(a, b);
					if (!_mm256_testz_si256(diff, diff))
					{
						total_diff += 1;
						ret=1;
						goto END;
					}
				}
			}
		END:
			// float mse = total_error / (float)(BLOCK_SIZE * BLOCK_SIZE * CHANNELS);
			// block_flags[block_id++] = (mse > DIFF_THRESHOLD) ? 1 : 0;
			block_flags[block_id++] = total_diff > 0 ? 1 : 0;
		}
	}
	return ret;
}

int main()
{
	size_t image_size = WIDTH * HEIGHT * CHANNELS;
	uint8_t *image1 = (uint8_t *)_mm_malloc(image_size, 32);
	uint8_t *image2 = (uint8_t *)_mm_malloc(image_size, 32);
	int block_count = (WIDTH / BLOCK_WIDTH) * (HEIGHT / BLOCK_HEIGHT);
	int *diff_flags = (int *)calloc(block_count, sizeof(int));

	printf("开始程序\n");
	printf("图像大小：%dx%d\n", WIDTH, HEIGHT);
	printf("分块数：%d\n", block_count);

	// 初始化图像数据（模拟差异）
	memset(image1, 100, image_size); // 图像1填充100
	memset(image2, 100, image_size); // 图像2相同
	// 制造一个差异：将图像2的某个区块修改
	for (int y = 0; y < 65; ++y)
	{
		for (int x = 640; x < 641; ++x)
		{
			int idx = (y * WIDTH + x) * CHANNELS;
			image2[idx + 0] = 255;
			image2[idx + 1] = 0;
			image2[idx + 2] = 0;
		}
	}

	for (int y = 0; y < 65; ++y)
	{
		for (int x = 100; x < 164; ++x)
		{
			int idx = (y * WIDTH + x) * CHANNELS;
			image2[idx + 0] = 255;
			image2[idx + 1] = 0;
			image2[idx + 2] = 0;
		}
	}

	// Timing variables
	struct timespec start, end;
	double elapsed_ms;

	// Get start time
	clock_gettime(CLOCK_MONOTONIC, &start);
	for (size_t i = 0; i < 1; i++)
	{
		compare_rgb_blocks_avx2(image1, image2, WIDTH, HEIGHT, diff_flags);
		//memcmp(image1, image2, WIDTH*HEIGHT*3); // 再次执行并比较
	}
	// Get end time
	clock_gettime(CLOCK_MONOTONIC, &end);
	// Calculate elapsed time in milliseconds
	elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0;
	elapsed_ms += (end.tv_nsec - start.tv_nsec) / 1000000.0;
	
	printf("执行时间: %.2f ms\n", elapsed_ms);
	// 输出差异区块结果
	int blocks_x = WIDTH / BLOCK_WIDTH;
	for (int i = 0; i < block_count; ++i)
	{
		if (diff_flags[i])
		{
			int row = i / blocks_x;
			int col = i % blocks_x;
			printf("差异区块位置：(%d, %d)\n", col, row);
		}
	}

	_mm_free(image1);
	_mm_free(image2);
	free(diff_flags);
	printf("结束程序\n");
	return 0;
}
