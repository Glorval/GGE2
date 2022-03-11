#version 330 core
layout(location = 0) in vec3 corePos;
layout(location = 1) in vec3 aColor;

uniform mat4 perspective;

uniform vec3 cordinates;
uniform vec4 orientation;
uniform float scale;
uniform vec4 cameraOrientation;

out vec3 fColor;

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


	//vec4 orientationNorm = normalizeQuat(orientation);
		
	vec4 orientated = quatMult(quatMult(orientation, cordFour), quatConj(orientation));


	//Notes: Clipping problem was because Z goes -1, 1, right? But I was double purposing Z as perspective number as well
	//so when trying to rotate it'd go all fucky wucky
	vec3 worldPos = orientated.yzw + cordinates;

	//temp.z = temp.z + cordinates.y;
	//temp.y = temp.y + cordinates.x;
	gl_Position = perspective * quatMult(vec4(worldPos, 1), cameraOrientation);
	fColor = aColor;
}