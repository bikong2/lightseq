/* Copyright 2021 The LightSeq Team
   Copyright NVIDIA FasterTransformer
   This file is adapted from NVIDIA FasterTransformer
*/
#pragma once

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <fstream>
#include <vector>
#include <cuda.h>
#include <cuda_runtime.h>
#include <cublasLt.h>
#include "util.h"

namespace lightseq {
namespace cuda {

#define STRIDE 32
#define BORDER 512

static std::string DEFAULT_URL = "https://zenodo.org/record/7176939/files/";
static std::string DEFAULT_DIR =
    std::string(std::getenv("HOME")) + "/.lightseq/igemm_configs/";
static std::string IGEMM_T4_CONFIG = "igemm_T4.cfg";
static std::string IGEMM_A100_CONFIG = "igemm_A100.cfg";
static std::string IGEMM_A30_CONFIG = "igemm_A30.cfg";
static std::string IGEMM_A10_CONFIG = "igemm_A10.cfg";

typedef struct {
  int algoId, customOption, tile, splitK_val, swizzle, reductionScheme,
      workspaceSize, stages;
  float fp16_time, int8_time, speedup;
  std::string dataOrder;
} cublasLtMatmulAlgo_info;

class cublasAlgoMap {
 private:
  std::map<std::vector<int>, std::map<std::string, cublasLtMatmulAlgo_info> >
      _algo_map;
  std::string _config_filename;
  int _workspace_size;
  void* _workspace = NULL;

 public:
  explicit cublasAlgoMap(const std::string filename);
  cublasAlgoMap();
  cublasAlgoMap(const cublasAlgoMap& map);
  ~cublasAlgoMap();

  bool fileExist(std::string path);
  void getGemmConfig();
  void loadGemmConfig();
  bool isExist(int m, int n, int k);
  cublasLtMatmulAlgo_info defaultAlgo();
  cublasLtMatmulAlgo_info findBestAlgo(
      std::map<std::string, cublasLtMatmulAlgo_info> mp,
      std::string data_order);
  cublasLtMatmulAlgo_info getAlgo(int m, int n, int k,
                                  std::string data_order = "");
  void* get_workspace();
  int get_workspace_size();
};

}  // namespace cuda
}  // namespace lightseq
