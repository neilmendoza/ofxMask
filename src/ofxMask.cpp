/*
 *  ofxMask.cpp
 *
 *  Copyright (c) 2011, Neil Mendoza, http://www.neilmendoza.com
 *  All rights reserved. 
 *  
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions are met: 
 *  
 *  * Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer. 
 *  * Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 *  * Neither the name of 16b.it nor the names of its contributors may be used 
 *    to endorse or promote products derived from this software without 
 *    specific prior written permission. 
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 *  POSSIBILITY OF SUCH DAMAGE. 
 *
 */
#include "ofxMask.h"
namespace Apex
{
	void ofxMask::setup(int width, int height)
	{
		fbo.allocate(width, height, GL_RGBA, ofFbo::maxSamples());
		string fragShaderSrc = 
		"#extension GL_ARB_texture_rectangle : enable\n"
		"uniform sampler2DRect mask;"
		"uniform sampler2DRect tex;"
		"void main()"
		"{"
		"	vec3 col = texture2DRect(tex, gl_TexCoord[0].st).rgb;"
		"	float alpha = texture2DRect(mask, gl_TexCoord[0].st).a;"
		"	gl_FragColor = vec4(col, alpha);"
		"}";
		shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSrc);
		shader.linkProgram();		
	}
	
	void ofxMask::beginMask()
	{
		fbo.begin();
		ofClear(0,0,0,0);
	}
	
	void ofxMask::endMask()
	{
		fbo.end();
	}
	
	void ofxMask::drawEffect(ofTexture& maskedImage)
	{
		shader.begin();
		shader.setUniformTexture("mask", fbo, 1);
		shader.setUniform1i("tex", 0);
		ofEnableAlphaBlending();
		maskedImage.draw(0, 0);
		shader.end();
	}	
}