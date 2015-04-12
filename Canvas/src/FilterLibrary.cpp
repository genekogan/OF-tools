#include "FilterLibrary.h"


void FilterLibrary::setup(int width, int height)
{
    Modifier::setup(width, height);
    
    ofDisableArbTex();
    ofEnableSmoothing();
    ofEnableAlphaBlending();

    _currentFilter = 0;
    
    fboInner = new ofFbo();
    fboInner->allocate(1280, 720);
    fboInner->begin();
    ofClear(0, 0);
    fboInner->end();
    
    // here's a simple filter chain
    
    FilterChain * charcoal = new FilterChain(width, height, "Charcoal");
    charcoal->addFilter(new BilateralFilter(width, height, 4, 4));
    charcoal->addFilter(new GaussianBlurFilter(width, height, 2.f ));
    charcoal->addFilter(new DoGFilter(width, height, 12, 1.2, 8, 0.99, 4));
    _filters.push_back(charcoal);
    
    // Basic filter examples
    _filters.push_back(new HalftoneFilter(width, height, 0.001));
    _filters.push_back(new CrosshatchFilter(width, height));
    _filters.push_back(new KuwaharaFilter(6));
    _filters.push_back(new SobelEdgeDetectionFilter(width, height));
    _filters.push_back(new BilateralFilter(width, height));
    _filters.push_back(new SketchFilter(width, height));
    _filters.push_back(new DilationFilter(width, height));
    _filters.push_back(new PerlinPixellationFilter(width, height));
    _filters.push_back(new XYDerivativeFilter(width, height));
    _filters.push_back(new ZoomBlurFilter());
    _filters.push_back(new EmbossFilter(width, height, 2.f));
    _filters.push_back(new SmoothToonFilter(width, height));
    _filters.push_back(new TiltShiftFilter(fboInner->getTextureReference()));
    _filters.push_back(new VoronoiFilter(fboInner->getTextureReference()));
    _filters.push_back(new CGAColorspaceFilter());
    _filters.push_back(new ErosionFilter(width, height));
    _filters.push_back(new LookupFilter(width, height, "img/lookup_amatorka.png"));
    _filters.push_back(new LookupFilter(width, height, "img/lookup_miss_etikate.png"));
    _filters.push_back(new LookupFilter(width, height, "img/lookup_soft_elegance_1.png"));
    _filters.push_back(new VignetteFilter());
    _filters.push_back(new PosterizeFilter(8));
    _filters.push_back(new LaplacianFilter(width, height, ofVec2f(1, 1)));
    _filters.push_back(new PixelateFilter(width, height));
    _filters.push_back(new HarrisCornerDetectionFilter(fboInner->getTextureReference()));
    _filters.push_back(new MotionDetectionFilter(fboInner->getTextureReference()));
    _filters.push_back(new LowPassFilter(width, height, 0.9));
    
    // blending examples
    
    ofImage wes = ofImage("img/wes.jpg");
    _filters.push_back(new DisplacementFilter("img/mandel.jpg", width, height, 25.f));
    _filters.push_back(new PoissonBlendFilter(wes.getTextureReference(), width, height, 2.0));
    _filters.push_back(new DisplacementFilter("img/glass/3.jpg", width, height, 40.0));
    _filters.push_back(new ExclusionBlendFilter(wes.getTextureReference()));
    
    
    // here's how you use Convolution filters
    
    Abstract3x3ConvolutionFilter * convolutionFilter1 = new Abstract3x3ConvolutionFilter(width, height);
    convolutionFilter1->setMatrix(-1, 0, 1, -2, 0, 2, -1, 0, 1);
    _filters.push_back(convolutionFilter1);
    
    Abstract3x3ConvolutionFilter * convolutionFilter2 = new Abstract3x3ConvolutionFilter(width, height);
    convolutionFilter2->setMatrix(4, 4, 4, 4, -32, 4, 4,  4, 4);
    _filters.push_back(convolutionFilter2);
    
    Abstract3x3ConvolutionFilter * convolutionFilter3 = new Abstract3x3ConvolutionFilter(width, height);
    convolutionFilter3->setMatrix(1.2,  1.2, 1.2, 1.2, -9.0, 1.2, 1.2,  1.2, 1.2);
    _filters.push_back(convolutionFilter3);
    
    
    // and here's how you might daisy-chain a bunch of filters
    
    FilterChain * foggyTexturedGlassChain = new FilterChain(width, height, "Weird Glass");
    foggyTexturedGlassChain->addFilter(new PerlinPixellationFilter(width, height, 13.f));
    foggyTexturedGlassChain->addFilter(new EmbossFilter(width, height, 0.5));
    foggyTexturedGlassChain->addFilter(new GaussianBlurFilter(width, height, 3.f));
    _filters.push_back(foggyTexturedGlassChain);
    
    
    // here's another unimaginative filter chain
    
    FilterChain * watercolorChain = new FilterChain(width, height, "Monet");
    watercolorChain->addFilter(new KuwaharaFilter(9));
    watercolorChain->addFilter(new LookupFilter(width, height, "img/lookup_miss_etikate.png"));
    watercolorChain->addFilter(new BilateralFilter(width, height));
    watercolorChain->addFilter(new PoissonBlendFilter("img/canvas_texture.jpg", width, height, 2.0));
    watercolorChain->addFilter(new VignetteFilter());
    _filters.push_back(watercolorChain);
    
    
    // and here's a random gradient map for posterity
    
    vector<GradientMapColorPoint> colors;
    for (float percent=0.0; percent<=1.0; percent+= 0.1)
        colors.push_back( GradientMapColorPoint(ofRandomuf(),ofRandomuf(),ofRandomuf(),percent) );
    _filters.push_back(new GradientMapFilter(colors));
    
    
    panel.setName("FilterLib");
    GuiMenu *menu = panel.addMenu("Select filter", this, &FilterLibrary::selectFilter);
    for (auto f : _filters) {
        menu->addToggle(f->getName());
    }
}

GuiPanel & FilterLibrary::getControl()
{
    return panel;
}

void FilterLibrary::render(ofFbo *fbo)
{
    fboInner->begin();
    fbo->draw(0, 0);
    fboInner->end();
    
    _filters[_currentFilter]->begin();
    fboInner->draw(0, 0);
    _filters[_currentFilter]->end();
}
