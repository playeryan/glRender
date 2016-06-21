#pragma once
#ifndef SHADERFILES_H
#define SHADERFILES_H

#include <cstdio>
#include <string>

#define ShaderFilesFolder	"ShaderFiles/"
const std::string LightVSFileName = std::string(ShaderFilesFolder"renderVertexShader.vert");
const std::string LightFSFileName = std::string(ShaderFilesFolder"renderFragmentShader.frag");
const std::string DirLightVSFileName = std::string(ShaderFilesFolder"directionalLightVertexShader.vert");
const std::string DirLightFSFileName = std::string(ShaderFilesFolder"directionalLightFragmentShader.frag");
const std::string PointLightVSFileName = std::string(ShaderFilesFolder"pointLightVertexShader.vert");
const std::string PointLightFSFileName = std::string(ShaderFilesFolder"pointLightFragmentShader.frag");
const std::string SpotLightVSFileName = std::string(ShaderFilesFolder"spotLightVertexShader.vert");
const std::string SpotLightFSFileName = std::string(ShaderFilesFolder"spotLightFragmentShader.frag");

const std::string ShadowVSFileName = std::string(ShaderFilesFolder"shadowMapVertexShader.vert");
const std::string ShadowFSFileName = std::string(ShaderFilesFolder"shadowMapFragmentShader.frag");
const std::string GBufferVSFileName = std::string(ShaderFilesFolder"GeometryBufferVertexShader.vert");
const std::string GBufferFSFileName = std::string(ShaderFilesFolder"GeometryBufferFragmentShader.frag");

const std::string NoneVSFileName = std::string(ShaderFilesFolder"NoneVertexShader.vert");
const std::string NoneFSFileName = std::string(ShaderFilesFolder"NoneFragmentShader.frag");

const std::string DeferredVSFileName = std::string(ShaderFilesFolder"deferredVertexShader.vert");
const std::string DeferredFSFileName = std::string(ShaderFilesFolder"deferredFragmentShader.frag");

#endif // !SHADERFILES_H

