/*
 * Unit Tests for Texture operations
 */

#include <cstdio>
#include <cstdlib>
#include <gtest/gtest.h>

#include "../src/jvisu/jvisu_test.h"


using namespace jvisu;


// Class of textures which leave a record of when they are freed

class RecordTexture : public TextureSolid {
public:
	bool *record;
	
	RecordTexture(Window *w, bool *r):
		TextureSolid(256, 256, w, 0xff, 0xff, 0xff, 0xff),
		record(r)
	{
		*record = true;
	}
	
	~RecordTexture(){
		*record = false;
	}
};




TEST(Texture, RecordTextureControl){
	/**
	 * This test just makes sure that the record texture system for these tests
	 * actually works.
	 */
	
	Window *window = new Window(100, 100, false);
	
	int texture_count = 5;
	bool *record = (bool*) malloc(texture_count * sizeof(bool));
	RecordTexture **textures = (RecordTexture**) malloc(texture_count * sizeof(RecordTexture*));
	
	// Create Textures
	for(int i = 0; i < texture_count; i++){
		textures[i] = new RecordTexture(window, record + i);
	}
	
	// Verify that bools are filled in correctly
	for(int i = 0; i < texture_count; i++){
		EXPECT_EQ(true, record[i]);
	}
	
	// Delete the textures
	for(int i = 0; i < texture_count; i++){
		delete textures[i];
		for(int j = 0; j < texture_count; j++){
			if(j <= i){
				EXPECT_EQ(false, record[j]);
			}else{
				EXPECT_EQ(true, record[j]);
			}
		}
	}
	
	free(textures);
	free(record);
	
	delete window;
}


TEST(Texture, SingleOwner){
	/**
	 * Attaches three textures to a single texture owner, detaches one then
	 * deletes the owner.
	 */
	
	Window *window = new Window(100, 100, false);
	
	
	int texture_count = 3;
	
	bool *record = (bool*) malloc(3 * sizeof(bool));
	RecordTexture **textures = (RecordTexture**) malloc(3 * sizeof(RecordTexture*));
	for(int i = 0; i < 3; i++){
		textures[i] = new RecordTexture(window, record + i);
	}
	
	
	// Verify that bools are filled in correctly
	for(int i = 0; i < texture_count; i++){
		EXPECT_EQ(true, record[i]);
	}
	
	
	TextureCache *owner = new TextureCache();
	for(int i = 0; i < texture_count; i++){
		owner->addTexture(textures[i]);
		EXPECT_EQ(true, record[i]);
	}
	
	EXPECT_EQ(owner->textureCount(), texture_count);
	
	
	// Delete one texture
	delete textures[0];
	for(int i = 1; i < texture_count; i++){
		EXPECT_EQ(true, record[i]);
	}
	EXPECT_EQ(false, record[0]);
	EXPECT_EQ(owner->textureCount(), texture_count - 1);
	
	
	// Remove one owner with no others
	owner->removeTexture(textures[1]);
	
	for(int i = 2; i < texture_count; i++){
		EXPECT_EQ(true, record[i]);
	}
	EXPECT_EQ(false, record[0]);
	EXPECT_EQ(false, record[1]);
	EXPECT_EQ(owner->textureCount(), texture_count - 2);
	
	
	// Delete the owner; all textures should be freed 
	delete owner;
	for(int i = 0; i < texture_count; i++){
		EXPECT_EQ(false, record[i]);
	}
	
	
	
	
	free(textures);
	free(record);
	
	delete window;
}



