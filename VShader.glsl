#version 330 core
layout(location = 0) in vec3 corePos;
layout(location = 1) in vec3 aColor;

uniform mat4 perspective;

uniform vec3 cordinates; //Of the object
uniform vec4 orientation; //of the object
uniform vec3 cameraPosition;
uniform vec4 cameraOrientation;

uniform float scale;
uniform float windowRatio;
uniform vec4 vectorColours;
uniform int flags;//Currently only for rendering mode

out vec4 fColor;

vec4 quatMult(vec4 o, vec4 c){
	vec4 temp;
	temp.x = (o.x * c.x) - (o.y * c.y) - (o.z * c.z) - (o.w * c.w);
	temp.y = (o.x * c.y) + (o.y * c.x) + (o.z * c.w) - (o.w * c.z);
	temp.z = (o.x * c.z) - (o.y * c.w) + (o.z * c.x) + (o.w * c.y);
	temp.w = (o.x * c.w) + (o.y * c.z) - (o.z * c.y) + (o.w * c.x);
	return(temp);
}

vec4 quatConj(vec4 q){
	q.y = -q.y;
	q.z = -q.z;
	q.w = -q.w;
	return(q);
}

vec4 normalizeQuat(vec4 quat) {
	float magnitude = sqrt((quat[0] * quat[0]) + (quat[1] * quat[1]) + (quat[2] * quat[2]) + (quat[3] * quat[3]));

	quat.x /= magnitude;
	quat.y /= magnitude;
	quat.z /= magnitude;
	quat.w /= magnitude;
	return(quat);
}

void main() {
	
	vec4 cordFour;
	cordFour.x = 0;
	cordFour.y = corePos.x;
	cordFour.z = corePos.y;
	cordFour.w = corePos.z;


	if(scale > 0.00001){
		cordFour = vec4(0, cordFour.y * scale, cordFour.z * scale, cordFour.w * scale);
	}
	
	//Standard rendering, good for most things
	if(flags == 0){
		vec4 orientated = quatMult(quatMult(orientation, cordFour), quatConj(orientation));
		vec3 worldPos = orientated.yzw + cordinates - cameraPosition;
		vec4 reposToCam = vec4(0, worldPos);
		reposToCam =  quatMult(quatMult(cameraOrientation, reposToCam), quatConj(cameraOrientation));

		gl_Position = perspective * vec4(reposToCam.yzw, 1);
		fColor = vec4(aColor, 1);
	}
	//Just position offsetting, good for UI
	else if(flags == 1){
		gl_Position = vec4(corePos.x + cordinates.x, corePos.y + cordinates.y, corePos.z + cordinates.z, 1);
		fColor = vec4(aColor, 1);
	}
	//Vector graphics mode
	else if(flags == 2){
		if(scale > 0.00001){
			cordFour = vec4(cordFour.x, cordFour.y * scale, cordFour.z * scale, cordFour.w * scale);
		}
		vec4 orientated = quatMult(quatMult(orientation, cordFour), quatConj(orientation));
		vec3 worldPos = orientated.yzw + cordinates - cameraPosition;
		
		vec4 reposToCam = vec4(0, worldPos);
		reposToCam =  quatMult(quatMult(cameraOrientation, reposToCam), quatConj(cameraOrientation));
		reposToCam.y = reposToCam.y * windowRatio;
		//vec4 windowCompensator = vec4(reposToCam.yzw, 1);
		//windowCompensator.x = windowCompensator.x * windowRatio;
		gl_Position =  perspective * vec4(reposToCam.yzw, 1);//windowCompensator;
		fColor = vectorColours;
	}
	//Vector graphics barebones mode- uses colours uniform rather than the location colour
	else if(flags == 3){
		vec2 fffffff = vec2(corePos.x, corePos.y);
		if(scale > 0.00001){
			fffffff.x *= scale;
			fffffff.y *= scale;
		}
		gl_Position = vec4((fffffff.x * windowRatio) + cordinates.x, fffffff.y + cordinates.y, corePos.z + cordinates.z, 1);
		fColor = vectorColours;
	}
	
}