from constants import intTypes, primitives

def parseDefinition(lines):
    types = []
    currentType = None

    def isEmpty(line):
        return len(line) < 2 # 0 cant happen, 1 is just \n

    def getIndent(line):
        for i in range(len(line)):
            if line[i] != '\t':
                return i
        return 0

    def getCurrentFields(type, indent):
        if indent == 1:
            return type['fields']
        else:
            return getCurrentFields(type['fields'][-1], indent - 1)

    for line in lines:
        line = line.replace('    ', '\t')
        empty = isEmpty(line)
        indent = getIndent(line)
        name = line[indent:-1]
        field = {
            'name': name,
            'fields': []
        } if name not in primitives else {
            'name': name,
        }

        IgnoreLine       =     empty and currentType is     None
        CloseCurrent     =     empty and currentType is not None
        CreateNewCurrent = not empty and currentType is     None
        UpdateCurrent    = not empty and currentType is not None

        if CloseCurrent:
            types += [currentType]
            currentType = None
        elif CreateNewCurrent:
            currentType = field
        elif UpdateCurrent:
            fields = getCurrentFields(currentType, indent)
            fields += [field]

    typenames = [t['name'] for t in types]

    def identifyDependencies(fields):
        dependencies = []
        for f in fields:
            name = f['name']
            if 'fields' in f:
                dependencies += identifyDependencies(f['fields'])
            for typename in typenames:
                if name == typename or name == typename + '[]':
                    dependencies += [typename]
        return dependencies

    deps = [identifyDependencies(t['fields']) for t in types]

    def swap(arr, i, j):
        arr[i], arr[j] = arr[j], arr[i]

    def depends(t1, t2):
        for i, t in enumerate(types):
            if t['name'] == t1:
                return t2 in deps[i]

    LOOP = 0
    while True:
        if LOOP > 100:
            print("You have a recursive type somewhere, assuming its fine, breaking")
            break
        LOOP += 1
        swappedAny = False
        for i in range(len(types)):
            for j in range(i + 1, len(types)):
                if depends(types[i]['name'], types[j]['name']):
                    swap(types, i, j)
                    swap(deps , i ,j)
                    swappedAny = True
        if not swappedAny:
            break

    return types
