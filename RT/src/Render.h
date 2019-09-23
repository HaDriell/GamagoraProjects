#pragma once

#include "Ray.h"
#include "Scene.h"
#include "Intersect.h"
#include "Framebuffer.h"
#include "Camera.h"


void render(const Scene& scene, Framebuffer& framebuffer, const Camera& camera);