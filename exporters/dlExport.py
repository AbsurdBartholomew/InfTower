import bpy
import struct
import sys
import bmesh

omsh_version = 0
hash = 362966967

def get_valid_scene_objects():
    object_list = []
    for object in bpy.data.objects:
        if object.type == "MESH":
            object_list.append(object)
    
    return object_list

def write_atomic(file, object):
    
    offset_size = (4 * 3)
    

    if object.type == "MESH":
        name = object.data.name
        data = bpy.data.meshes[name]
        material = object.material_slots[0].material
        
        #file.write(bytes(name, 'utf-8'))
        #file.write(bytes(chr(0), 'utf-8'))

        file.write(struct.pack('i', hash))
        file.write(struct.pack('i', omsh_version))

        # write first material
        texture = material.texture_slots[0].texture.image
        print(texture.name)
        
        file.write(struct.pack('i', len(texture.name)))
        print(len(texture.name))
        file.write(bytes(texture.name, 'utf-8'))
        #file.write(bytes(chr(0), 'utf-8'))

        bm = bmesh.new()
        bm.from_mesh(data)
        bm.calc_tessface()
        bmesh.ops.triangulate(bm, faces=bm.faces)
        bm.normal_update()
        # write atomic header
        
        #data_size = (len(bm.faces) * (4 * 3)) + (len(bm.faces) * (4 * 3)) + offset_size# + (len(data.uv_layers[0].data) * (4 * 2))        
        #file.write(struct.pack('>i', data_size))
        file.write(struct.pack('i', len(bm.faces)))
        #file.write(struct.pack('>i', len(data.vertices)))
        
        for face in bm.faces:
            for vertex in face.verts:
                file.write(struct.pack('fff', vertex.normal[0], vertex.normal[1], vertex.normal[2]))
                file.write(struct.pack('fff', vertex.co[0], vertex.co[1], vertex.co[2]))

        test_counter = 0

        for poly in data.polygons:
            for loop_index in poly.loop_indices:
                uv = object.data.uv_layers[0].data[loop_index].uv
                file.write(struct.pack('ff', uv[0], uv[1]))
                test_counter += 1

        print("TEST COUNTER:", test_counter)
        
        
    
    #file.write(struct.pack('fff', object.location[0], object.location[1], object.location[2]))

def write_some_data(context, filepath, use_some_setting):
    print("running write_some_data...")
    f = open(filepath, 'wb')
    #objects = get_valid_scene_objects()
    write_atomic(f, bpy.context.selected_objects[0])
    
    #print(len(objects))

    #f.write(struct.pack('>i', len(objects)))
    #for atomic in objects:
    #    print("found", atomic.name)
    #    write_atomic(f, atomic)

    f.close()

    return {'FINISHED'}


# ExportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty
from bpy.types import Operator


class ExportGLDL(Operator, ExportHelper):
    """Export current scene to a DL file (Inf Tower GL Display list)"""
    bl_idname = "export_gl.dl"  # important since its how bpy.ops.import_test.some_data is constructed
    bl_label = "Export GL DL"

    # ExportHelper mixin class uses this
    filename_ext = ".dl"

    filter_glob = StringProperty(
        default="*.dl",
        options={'HIDDEN'},
        maxlen=255,  # Max internal buffer length, longer would be clamped.
    )

    # List of operator properties, the attributes will be assigned
    # to the class instance from the operator settings before calling.
    use_setting = BoolProperty(
        name="Example Boolean",
        description="Example Tooltip",
        default=True,
    )

    type = EnumProperty(
        name="Example Enum",
        description="Choose between two items",
        items=(
            ('OPT_A', "First Option", "Description one"),
            ('OPT_B', "Second Option", "Description two"),
        ),
        default='OPT_A',
    )

    def execute(self, context):
        return write_some_data(context, self.filepath, self.use_setting)


# Only needed if you want to add into a dynamic menu
def menu_func_export(self, context):
    self.layout.operator(ExportGLDL.bl_idname, text="Export Inf Tower GL Display List (.dl)")


def register():
    bpy.utils.register_class(ExportGLDL)
    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportGLDL)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()

    # test call
    bpy.ops.export_gl.dl('INVOKE_DEFAULT')